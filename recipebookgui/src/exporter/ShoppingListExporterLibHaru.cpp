#include "ShoppingListExporterLibHaru.h"
#include <QException>
#include <QFile>
#include <data/GoShoppingListItem.h>
#include <data/SortedShoppingList.h>
#include "../uistringconverter.h"

#include "LibHaruCommon.h"

using namespace recipebook;

namespace
{
	const float c_fontSize = 10;

	QString formatItem(const UIStringConverter& converter, const GoShoppingListItem& rItem)
	{
		if(rItem.getAmount().size() == 1)
		{
			QString strAmount = converter.formatAmount(rItem.getAmount().at(0));
			if(!strAmount.isEmpty())
			{
				strAmount = " " + strAmount;
			}
			return strAmount + " " + rItem.getName();
		}
		else if(rItem.getAmount().size() > 1)
		{
			QString strText;
			for(Amount amount : rItem.getAmount())
			{
				if(!strText.isEmpty())
				{
					strText += " + ";
				}
				strText += converter.formatAmount(amount, false);
			}
			if(!strText.isEmpty())
			{
				strText = " " + strText;
			}
			return strText + " " + rItem.getName();
		}

		return "";
	}

	void addPageIfNeeded(HPDF_Doc document, HPDF_Page& page, float& currentX, float& rCurrentY)
	{
		if(rCurrentY <= c_border)
		{
			if(currentX > 0.0)
			{
				// Add new page

				page = HPDF_AddPage(document);
				HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

				currentX = 0.0;
			}
			else
			{
				// Second column
				auto pageWidth = HPDF_Page_GetWidth(page);
				currentX = pageWidth / 2.0f;
			}

			auto pageHeight = HPDF_Page_GetHeight(page);
			rCurrentY = pageHeight - c_border;
		}
	}

	void addItemAdditionalText(const GoShoppingListItem& rItem,
							   HPDF_Doc document,
							   HPDF_Page& page,
							   float startX,
							   float& currentX,
							   float& currentY,
							   float indentLength,
							   float lineHeight,
							   float titleLength,
							   const UIStringConverter& converter)
	{
		HPDF_Page_SetGrayFill(page, 0.5f);

		auto pageWidth = HPDF_Page_GetWidth(page);

		if(rItem.getCombinedItemsCount() == 1)
		{
			QString addText;
			if(rItem.getSize(0) != Size::Normal)
			{
				addText += converter.convertSize(rItem.getSize(0), rItem.getAmount(0).getUnit());
			}
			if(!rItem.getAdditionalInfo(0).isEmpty())
			{
				if(!addText.isEmpty())
				{
					addText += ", ";
				}
				addText += rItem.getAdditionalInfo(0);
			}

			if(!addText.isEmpty())
			{
				QString text = " (" + addText + ")";
				auto strAddText(convertString(text));

				float addTextLength = HPDF_Page_TextWidth(page, strAddText.data());

				if(titleLength + addTextLength < pageWidth / 2.0f - startX)
				{
					HPDF_Page_BeginText(page);
					HPDF_Page_TextOut(page,
									currentX + startX + titleLength,
									currentY,
									strAddText.data());
					HPDF_Page_EndText(page);
				}
				else
				{
					HPDF_Page_BeginText(page);
					HPDF_Page_TextOut(page,
									currentX + startX + 1.5f * indentLength,
									currentY - 2.0f * lineHeight / 3.0f,
									strAddText.data());
					HPDF_Page_EndText(page);
					currentY -= 2 * lineHeight / 3;

					addPageIfNeeded(document, page, currentX, currentY);
				}
			}
		}
		else if(rItem.getCombinedItemsCount() > 1)
		{
			for(quint32 i = 0; i < rItem.getCombinedItemsCount(); ++i)
			{
				QString strText;
				if(rItem.getSize(i) != Size::Normal)
				{
					if(!strText.isEmpty())
					{
						strText += ", ";
					}
					strText += converter.convertSize(rItem.getSize(i), rItem.getAmount(i).getUnit());
				}

				if(rItem.isOptional(i))
				{
					if(!strText.isEmpty())
					{
						strText += ", ";
					}
					strText += QObject::tr("Optional");
				}

				if(!rItem.getAdditionalInfo(i).isEmpty())
				{
					if(!strText.isEmpty())
					{
						strText += ", ";
					}
					strText += rItem.getAdditionalInfo(i);
				}

				if(!strText.isEmpty())
				{
					QString strAmount = converter.formatAmount(rItem.getAmount(i));
					strText = QString(QObject::tr("- %1 of those %2")).arg(strAmount).arg(strText);
				}
				else if(rItem.getAmount(i).getUnit() == Unit::Unitless)
				{
					strText = QString(QObject::tr("<li>Unitless amount for \"%1\"</li>")).arg(rItem.getRecipeInfos()[i].m_RecipeName);
				}

				if(!strText.isEmpty())
				{
					auto strAddText(convertString(strText));
					HPDF_Page_BeginText(page);
					HPDF_Page_TextOut(page,
									currentX + startX + 1.5f * indentLength,
									currentY - 2.0f * lineHeight / 3.0f,
									strAddText.data());
					HPDF_Page_EndText(page);
					currentY -= 2 * lineHeight / 3;

					addPageIfNeeded(document, page, currentX, currentY);
				}
			}
		}

		HPDF_Page_SetGrayFill(page, 0.0f);
	}
}

bool recipebook::exportShoppingListLibHaru(const UIStringConverter& converter, const SortedShoppingList& list, QString filename)
{
	HPDF_Doc document;

	try
	{
		document = HPDF_New (error_handler, NULL);

		HPDF_UseUTFEncodings(document);

		HPDF_SetPageMode (document, HPDF_PAGE_MODE_USE_OUTLINE);

		// Metainformation
		HPDF_SetInfoAttr(document, HPDF_INFO_AUTHOR, convertString(c_author).data());
		HPDF_SetInfoAttr(document, HPDF_INFO_CREATOR, convertString(convertString(QObject::tr("Shopping list"))).data());

		float indentLength = c_fontSize;
		float lineHeight = 1.5f * indentLength;

		// Images

		auto readImage = [&document](QString _strFilename) -> HPDF_Image
		{
			QFile f(_strFilename);
			f.open(QIODevice::ReadOnly);
			QByteArray ba = f.readAll();
			return HPDF_LoadPngImageFromMem(document, reinterpret_cast<HPDF_BYTE*>(ba.data()), static_cast<HPDF_UINT>(ba.size()));
		};

		auto imageSquare = readImage(":/export-images/square.png");

		float scaleSquare = indentLength;

		// Pages

		HPDF_Page page = HPDF_AddPage(document);
		HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

		auto pageHeight = HPDF_Page_GetHeight(page);

		float currentY = pageHeight - c_border;

		auto boldFont = getFont(document, FontType::Bold);
		auto textFont = getFont(document, FontType::Regular);

		HPDF_Page_SetFontAndSize (page, boldFont, 4.0f * c_fontSize / 3.0f);

		// Header
		QString tocTitle = QObject::tr("Shopping list");
		auto pdfTocTitle(convertString(tocTitle));
		HPDF_Page_BeginText (page);
		HPDF_Page_TextOut(page, c_border, currentY, pdfTocTitle.data());
        HPDF_Page_EndText (page);

		currentY -= lineHeight;

		// Current row starting position on the page
		float currentX = 0.0;

		for(quint32 i = 0; i < list.getItemsCount(); ++i)
		{
			const GoShoppingListItem& item = list.getItemAt(i);

			QString strTitle;

			float startX = c_border;
			float deltaX = 0.0;
			if(item.getType() == GoShoppingListItemType::Category_Header)
			{
				currentY -= lineHeight / 3;
				HPDF_Page_SetFontAndSize (page, boldFont, c_fontSize);

				strTitle = item.getName();
			}
			else
			{
				startX += indentLength;
				HPDF_Page_SetFontAndSize (page, textFont, c_fontSize);

				HPDF_Page_DrawImage(page,
									imageSquare,
									currentX + startX,
									currentY - 0.2f * indentLength,
									scaleSquare,
									scaleSquare);
				deltaX += scaleSquare;

				strTitle = formatItem(converter, item);
			}

			auto pdfTitle(convertString(strTitle));

			HPDF_Page_BeginText(page);
			HPDF_Page_TextOut(page,
							currentX + startX + deltaX,
							currentY,
							pdfTitle.data());
			HPDF_Page_EndText(page);

			float titleLength = HPDF_Page_TextWidth(page, pdfTitle.data()) + deltaX;
			addItemAdditionalText(item,
								  document,
								  page,
								  startX,
								  currentX,
								  currentY,
								  indentLength,
								  lineHeight,
								  titleLength,
								  converter);

			currentY -= 2 * lineHeight / 3;

			addPageIfNeeded(document, page, currentX, currentY);
		}

		HPDF_SaveToFile (document, filename.toUtf8());
	}
	catch(...)
	{
		HPDF_Free (document);
		return false;
	}

	HPDF_Free (document);
	return true;

}
