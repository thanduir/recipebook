#include "RecipeBookExporterLibHaru.h"
#include <QException>
#include <QFile>
#include <data/RecipeBookConfiguration.h>
#include <data/RecipeBookConfigItem.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
#include <data/AlternativesType.h>
#include "../uistringconverter.h"

#include "LibHaruCommon.h"

using namespace recipebook;

namespace
{
	struct TocItem
	{
		QString						name;
		RecipeBookConfigItemType	type;
		qint32						level;
		HPDF_Destination			destination;

		TocItem(QString _name, RecipeBookConfigItemType _type, qint32 _level, HPDF_Destination _destination)
			: name(_name), type(_type), level(_level), destination(_destination) {}
	};

	void drawMultiLineText(HPDF_Page page, const char* text, float startX, float startY, float width, float height)
	{
		HPDF_Page_BeginText(page);
		HPDF_Page_TextRect(page, startX, startY + height, startX + width, startY, text, HPDF_TALIGN_LEFT, NULL);
		HPDF_Page_EndText(page);
	}

	void drawDot(HPDF_Page page, float x, float y, float fontSize)
	{
		HPDF_Page_Circle(page, x, y + 0.4f * fontSize, 0.15f * fontSize);
		HPDF_Page_FillStroke(page);
	}

	void addTitlePage(HPDF_Doc document, QString title, QString subtitle, quint32 defaultFontSize, const HPDF_Outline currentParentOutlineItem)
	{
		HPDF_Page page = HPDF_AddPage(document);
		HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

		auto textFont = getFont(document, FontType::Bold);

		auto height = HPDF_Page_GetHeight(page);
        auto width = HPDF_Page_GetWidth(page);

		auto dst = HPDF_Page_CreateDestination(page);
		HPDF_Destination_SetXYZ(dst, 0, HPDF_Page_GetHeight(page), 1);
		HPDF_Outline_SetDestination(currentParentOutlineItem, dst);

		// title
		auto strTitle = convertString(title);
		HPDF_Page_SetFontAndSize (page, textFont, static_cast<float>(defaultFontSize) * 1.6f);
        auto titleWidth = HPDF_Page_TextWidth (page, strTitle.data());
        HPDF_Page_BeginText (page);
		HPDF_Page_TextOut(page, (width - titleWidth) / 2.0f, height / 2.0f + c_border, strTitle.data());
        HPDF_Page_EndText (page);

		// subtitle
		auto strSubtitle = convertString(subtitle);
		HPDF_Page_SetFontAndSize (page, textFont, static_cast<float>(defaultFontSize) * 1.3f);
		auto subtitleWidth = HPDF_Page_TextWidth (page, strSubtitle.data());
        HPDF_Page_BeginText (page);
		HPDF_Page_TextOut(page, (width - subtitleWidth) / 2.0f, height / 2.0f, strSubtitle.data());
        HPDF_Page_EndText (page);
	}

	void addChapterHeader(HPDF_Doc document, QString title, qint32 level, quint32 defaultFontSize, HPDF_Outline& currentParentOutlineItem, QList<TocItem>& tocItems)
	{
		HPDF_Page page = HPDF_AddPage(document);
		HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

		auto height = HPDF_Page_GetHeight(page);

		// TODO: Adjust to level

		auto textFont = getFont(document, FontType::Bold);

		// Define page

		auto strTitle = convertString(title);
		HPDF_Page_SetFontAndSize (page, textFont, 4.0f * static_cast<float>(defaultFontSize) / 3.0f);

        HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, c_border, height - 2.0f * c_border, strTitle.data());
        HPDF_Page_EndText(page);

		// Create destination and add to outline and TOC

		auto dst = HPDF_Page_CreateDestination(page);
		HPDF_Destination_SetXYZ(dst, 0, HPDF_Page_GetHeight(page), 1);

		currentParentOutlineItem = HPDF_CreateOutline(document, currentParentOutlineItem, strTitle.data(), NULL);
		HPDF_Outline_SetDestination(currentParentOutlineItem, dst);

		tocItems.append(TocItem(title, RecipeBookConfigItemType::Header, level, dst));
	}

	void addRecipeItems(HPDF_Doc document,
						HPDF_Page page,
						HPDF_Font textFont,
						float defaultFontSize,
						float indentLength,
						float lineHeight,
						float minX,
						float currentY,
						const Recipe& recipe,
					    const UIStringConverter& converter)
	{
		auto boldFont = getFont(document, FontType::Bold);
		auto italicFont = getFont(document, FontType::Italic);

		float x = minX;
		float y = currentY;

		QString currentGroup;
		for(quint32 i = 0; i < recipe.getRecipeItemsCount(); ++i)
		{
			const RecipeItem& rItem = recipe.getRecipeItemAt(i);

			// End of group?
			if(!currentGroup.isEmpty()
			&& (!rItem.hasAlternativesGroup() || rItem.getAlternativesGroup().getName() != currentGroup))
			{
				// End current group
				x -= indentLength;
				currentGroup.clear();
			}

			// Begin of new group?
			if(rItem.hasAlternativesGroup() && rItem.getAlternativesGroup().getName() != currentGroup)
			{
				currentGroup = rItem.getAlternativesGroup().getName();

				HPDF_Page_SetFontAndSize (page, boldFont, defaultFontSize);
				drawDot(page, x, y, defaultFontSize);
				QString strGroup = QString("  %1").arg(rItem.getAlternativesGroup().getName());
				auto str(convertString(strGroup));
				HPDF_Page_BeginText(page);
				HPDF_Page_TextOut(page, x, y, str.data());
				HPDF_Page_EndText(page);

				x += indentLength;
				y -= lineHeight;
			}

			if(currentGroup.isEmpty())
			{
				drawDot(page, x, y, defaultFontSize);
			}
			QString itemText = !currentGroup.isEmpty() ? "- " : QString("  ");
			QString amount = converter.formatAmount(rItem.getAmount());
			if(!amount.isEmpty())
			{
				itemText += amount + " ";
			}// Create destination and add to outline and TOC

			itemText += rItem.getName();

			auto strItemText(convertString(itemText));
			if(rItem.isOptional())
			{
				HPDF_Page_SetFontAndSize (page, italicFont, defaultFontSize);
				HPDF_Page_BeginText(page);
				HPDF_Page_TextOut(page, x, y, strItemText.data());
				HPDF_Page_EndText(page);
			}
			else
			{
				HPDF_Page_SetFontAndSize (page, boldFont, defaultFontSize);
				HPDF_Page_BeginText(page);
				HPDF_Page_TextOut(page, x, y, strItemText.data());
				HPDF_Page_EndText(page);
			}

			float itemTextLength = HPDF_Page_TextWidth (page, strItemText.data());

			HPDF_Page_SetFontAndSize (page, textFont, defaultFontSize);

			QString addText;
			if(rItem.getSize() != Size::Normal)
			{
				addText += converter.convertSize(rItem.getSize(), rItem.getAmount().getUnit());
			}
			if(!rItem.getAdditionalInfo().isEmpty())
			{
				if(!addText.isEmpty())
				{
					addText += ", ";
				}
				addText += rItem.getAdditionalInfo();
			}
			if(!addText.isEmpty())
			{
				QString text = " (" + addText + ")";
				auto strAddText(convertString(text));

				float addTextLength = HPDF_Page_TextWidth (page, strAddText.data());

				HPDF_Page_BeginText(page);
				if(itemTextLength + addTextLength < HPDF_Page_GetWidth(page) / 2.0 - minX)
				{
					HPDF_Page_TextOut(page, x + itemTextLength, y, strAddText.data());
				}
				else
				{
					HPDF_Page_TextOut(page, x + indentLength, y - 1.25f * indentLength, strAddText.data());
					y -= 1.25f * indentLength;
				}
				HPDF_Page_EndText(page);
			}

			y -= lineHeight;
		}
	}

	void addRecipePage(HPDF_Doc document,
					   const Recipe& recipe,
					   quint32 defFontSize,
					   float indentLength,
					   float lineHeight,
					   HPDF_Image imgPersons,
					   HPDF_Image imgDuration,
					   const HPDF_Outline currentParentOutlineItem,
					   QList<TocItem>& tocItems,
					   const UIStringConverter& converter)
	{
		HPDF_Page page = HPDF_AddPage(document);
		HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

		auto pageHeight = HPDF_Page_GetHeight(page);
        auto pageWidth = HPDF_Page_GetWidth(page);

		float defaultFontSize = static_cast<float>(defFontSize);

		// Define page

		float currentY = pageHeight - 2.0f * c_border;

		// Header
		auto titleFont = getFont(document, FontType::Bold);
		auto textFont = getFont(document, FontType::Regular);

		float titleFontSize = 4.0f * defaultFontSize / 3.0f;
		HPDF_Page_SetFontAndSize (page, titleFont, titleFontSize);
		auto pdfTitle(convertString(recipe.getName()));

		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, c_border, currentY, pdfTitle.data());
		HPDF_Page_EndText(page);

		currentY -= lineHeight * 2.0f;
		HPDF_Page_SetFontAndSize (page, textFont, defaultFontSize);

		// Short desc, number of persons and duration

		auto pdfShortDesc(convertString(recipe.getShortDescription()));

		// Number of persons and duration
		QString strDuration;
		if(recipe.getCookingTime().isValid())
		{
			if(recipe.getCookingTime().hour() > 0)
			{
				strDuration = recipe.getCookingTime().toString("HH:mm") + QObject::tr("h");
			}
			else
			{
				strDuration = recipe.getCookingTime().toString("mm") + QObject::tr("min");
			}
		}

		float scalePersons = 1.1f * indentLength;
		QString strTextPersons = QString("%1").arg(recipe.getNumberOfPersons());
		auto pdfTextPersons(convertString(strTextPersons));

		float scaleDuration = 1.1f * indentLength;
		QString strTextDuration = QString("%1").arg(strDuration);
		auto pdfTextDuration(convertString(strTextDuration));

		float topRightLength = scaleDuration + HPDF_Page_TextWidth(page, pdfTextDuration.data());
		float twoRowHeight = 1.5f * lineHeight;

		// Short desc
		drawMultiLineText(page,
						  pdfShortDesc.data(),
						  c_border,
						  currentY - twoRowHeight / 2.0f,
						  pageWidth - 2.0f * c_border - topRightLength,
						  twoRowHeight);

		// Nr persons
		float startX = pageWidth - c_border - topRightLength;
		float startY = currentY + 0.5f * indentLength;
		HPDF_Page_DrawImage(page,
							imgPersons,
                            startX,
							startY,
							scalePersons,
							scalePersons);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page,
						  startX + scalePersons + indentLength * 0.5f,
						  startY + 0.25f * indentLength,
						  pdfTextPersons.data());
		HPDF_Page_EndText(page);

		// Duration
		float yDuration = startY - 1.25f * scalePersons;
		HPDF_Page_DrawImage(page,
							imgDuration,
                            startX,
							yDuration,
							scaleDuration,
							scaleDuration);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page,
						  startX + scaleDuration + indentLength * 0.5f,
						  yDuration + 0.25f * indentLength,
						  pdfTextDuration.data());
		HPDF_Page_EndText(page);

		currentY -= lineHeight;

		// Horizontal line
		//HPDF_Page_SetLineWidth (page, 0.5);
		HPDF_Page_MoveTo (page, pageWidth / 6.0f, currentY);
		HPDF_Page_LineTo (page, 5.0f * pageWidth / 6.0f, currentY);
		HPDF_Page_Stroke (page);

		currentY -= lineHeight;

		// Items
		addRecipeItems(document, page, textFont, defaultFontSize, indentLength, lineHeight, c_border, currentY - 12, recipe, converter);

		// Middle line
		HPDF_Page_MoveTo (page, pageWidth / 2.0f, currentY);
		HPDF_Page_LineTo (page, pageWidth / 2.0f, c_border);
		HPDF_Page_Stroke (page);


		auto pdfText(convertString(recipe.getRecipeText()));
		float textHeight = currentY - c_border;

		drawMultiLineText(page,
						  pdfText.data(),
						  pageWidth / 2.0f + indentLength,
						  c_border,
						  pageWidth / 2.0f - indentLength - c_border,
						  textHeight);

		// Create destination and add to outline and TOC

		auto dst = HPDF_Page_CreateDestination(page);
		HPDF_Destination_SetXYZ(dst, 0, HPDF_Page_GetHeight(page), 1);

		auto outlineItem = HPDF_CreateOutline(document, currentParentOutlineItem, pdfTitle.data(), NULL);
		HPDF_Outline_SetDestination(outlineItem, dst);

		tocItems.append(TocItem(recipe.getName(), RecipeBookConfigItemType::Recipe, -1, dst));
	}

	void addTOC(HPDF_Doc document,
				quint32 defFontSize,
				float indentLength,
				float lineHeight,
				QList<TocItem> tocItems)
	{
		unsigned int currentPageIndex = 1;

		auto page = HPDF_InsertPage(document, HPDF_GetPageByIndex(document, currentPageIndex));
		HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

		auto pageHeight = HPDF_Page_GetHeight(page);

		float defaultFontSize = static_cast<float>(defFontSize);

		float currentY = pageHeight - 2.0f * c_border;

		auto boldFont = getFont(document, FontType::Bold);
		auto textFont = getFont(document, FontType::Regular);

		// Header
		float titleFontSize = 4.0f * defaultFontSize / 3.0f;
		HPDF_Page_SetFontAndSize (page, boldFont, titleFontSize);

		QString tocTitle = QObject::tr("Table of contents");
		auto pdfTocTitle(convertString(tocTitle));

		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, c_border, currentY, pdfTocTitle.data());
		HPDF_Page_EndText(page);

		currentY -= lineHeight;

		qint32 currentLevel = 0;
		for(const TocItem& item : tocItems)
		{
			auto pdfTitle(convertString(item.name));

			float startX = c_border;
			if(item.type == RecipeBookConfigItemType::Recipe)
			{
				startX += indentLength;
				HPDF_Page_SetFontAndSize (page, textFont, defaultFontSize);
			}
			else
			{
				currentLevel = item.level;

				if(currentLevel == 0)
				{
					currentY -= lineHeight / 3;
				}
				HPDF_Page_SetFontAndSize (page, boldFont, defaultFontSize);
			}

			startX += static_cast<float>(currentLevel) * indentLength;

			HPDF_Page_BeginText(page);
			HPDF_Page_TextOut(page, startX, currentY, pdfTitle.data());
			HPDF_Page_EndText(page);

			HPDF_Rect rect;
			rect.left = startX;
			rect.top = currentY;
			rect.right = startX + HPDF_Page_TextWidth(page, pdfTitle.data());
			rect.bottom = currentY + defaultFontSize;
			auto annotation = HPDF_Page_CreateLinkAnnot (page, rect, item.destination);
			HPDF_LinkAnnot_SetBorderStyle(annotation, 0, 0, 0);

			currentY -= 2 * lineHeight / 3;

			if(currentY <= c_border)
			{
				// Add new page
				++currentPageIndex;

				page = HPDF_InsertPage(document, HPDF_GetPageByIndex(document, currentPageIndex));
				HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

				currentY = pageHeight - 2.0f * c_border;
			}
		}
	}
}

bool recipebook::exportRecipeBookLibHaru(const UIStringConverter& converter, const RecipeBookConfiguration& config, QString filename)
{
	HPDF_Doc document;

	try 
	{
		document = HPDF_New (error_handler, NULL);

		HPDF_UseUTFEncodings(document);

		HPDF_SetPageMode (document, HPDF_PAGE_MODE_USE_OUTLINE);

		// Metainformation
		HPDF_SetInfoAttr(document, HPDF_INFO_AUTHOR, convertString(c_author).data());
		HPDF_SetInfoAttr(document, HPDF_INFO_CREATOR, convertString(config.getBookSubtitle()).data());
		HPDF_SetInfoAttr(document, HPDF_INFO_TITLE, convertString(config.getBookTitle()).data());

		quint32 fontSize = config.getFontSize();

		float indentLength = static_cast<float>(config.getFontSize());
		float lineHeight = 2.25f * indentLength;
		
		HPDF_Outline outlineRoot = HPDF_CreateOutline(document, NULL, convertString(config.getBookTitle()).data(), NULL);
		HPDF_Outline_SetOpened(outlineRoot, HPDF_TRUE);
		HPDF_Outline currentParentOutlineItem = outlineRoot;

		// Images

		auto readImage = [&document](QString _strFilename) -> HPDF_Image
		{
			QFile f(_strFilename);
			f.open(QIODevice::ReadOnly);
			QByteArray ba = f.readAll();
			return HPDF_LoadPngImageFromMem(document, reinterpret_cast<HPDF_BYTE*>(ba.data()), static_cast<HPDF_UINT>(ba.size()));
		};

		auto imagePersons = readImage(":/export-images/persons.png");
		auto imageDuration = readImage(":/export-images/duration.png");

		QList<TocItem> tocItems;

		// Pages
		
		addTitlePage(document, config.getBookTitle(), config.getBookSubtitle(), fontSize, currentParentOutlineItem);

		for(quint32 i = 0; i < config.getItemsCount(); ++i)
		{
			const RecipeBookConfigItem& item = config.getItemAt(i);

			switch(item.getType())
			{
			case RecipeBookConfigItemType::Header:
			{
				if(item.getLevel() == 0)
				{
					currentParentOutlineItem = outlineRoot;
				}

				addChapterHeader(document, item.getName(), item.getLevel(), fontSize, currentParentOutlineItem, tocItems);

				break;
			}

			case RecipeBookConfigItemType::Recipe:
			{
				addRecipePage(document,
							  *item.getRecipe(),
							  fontSize,
							  indentLength,
							  lineHeight,
							  imagePersons,
							  imageDuration,
							  currentParentOutlineItem,
							  tocItems,
							  converter);
				break;
			}

			default:
			{
				throw QException();
			}
			}
		}

		addTOC(document, fontSize, indentLength, lineHeight, tocItems);

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
