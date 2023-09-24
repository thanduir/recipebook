#include "ShoppingListExporterPodofo.h"
#include <QException>
#include <QFile>
#include <data/GoShoppingListItem.h>
#include <data/SortedShoppingList.h>
#include "../uistringconverter.h"

#include "podofo.h"

using namespace recipebook;
using namespace PoDoFo;

namespace
{
	const QString c_Author = "RecipeBook";

	const double c_dFontSize = 10;
	const double c_dBorder = 56.69;

	double getLength(PdfFont* pFont, PdfString strText)
	{
		PdfTextState state;
		state.FontSize = c_dFontSize;
		return pFont->GetStringLength(strText, state);
	}

	PdfStandard14FontType getFontType(bool bBold, bool bItalic)
	{
		if(bBold && bItalic)
		{
			return PdfStandard14FontType::HelveticaBoldOblique;
		}
		else if(bBold)
		{
			return PdfStandard14FontType::HelveticaBoldOblique;
		}
		else if(bItalic)
		{
			return PdfStandard14FontType::HelveticaOblique;
		}
		else
		{
			return PdfStandard14FontType::Helvetica;
		}
	}
}

ShoppingListExporterPodofo::ShoppingListExporterPodofo(const UIStringConverter& rConverter)
	: m_rConverter(rConverter)
{
}

ShoppingListExporterPodofo::~ShoppingListExporterPodofo()
{
}

bool ShoppingListExporterPodofo::writeDocument(QString strFilename, const SortedShoppingList& rList)
{
	try 
	{
		m_spDocument = std::make_unique<PdfMemDocument>();

		// Metainformation
		if(m_spDocument->GetInfo() != nullptr)
		{
			m_spDocument->GetMetadata().SetCreator(convertString(c_Author));
			m_spDocument->GetMetadata().SetTitle(convertString(tr("Shopping list")));
		}

		m_dIndentLength = c_dFontSize;
		m_dLineHeight = 1.5 * m_dIndentLength;

		// Default fonts
		m_pTextFont = createFont(false, false);
		
		// Images

		auto readImage = [](QString _strFilename, PdfImage* image)
		{
			QFile f(_strFilename);
			f.open(QIODevice::ReadOnly);
			QByteArray ba = f.readAll();
			image->LoadFromBuffer(ba);
		};

		auto imgSquare = m_spDocument->CreateImage();
		readImage(":/export-images/square.png", imgSquare.get());
		double scaleSquare = m_dIndentLength / imgSquare->GetHeight();

		// Pages
		
		unsigned int currentPageIndex = 1;
		PdfPage* pPage = &m_spDocument->GetPages().CreatePageAt(currentPageIndex, PdfPage::CreateStandardPageSize(PdfPageSize::A4));

		double currentY = pPage->GetRect().Width - c_dBorder;

		PdfPainter painter;
		painter.SetCanvas(*pPage);

		// Header
		PdfFont* pTitleFont = createFont(true, false);
		painter.TextState.SetFont(*pTitleFont, 4.0 * c_dFontSize / 3.0);

		QString tocTitle = tr("Shopping list");
		PdfString pdfTocTitle(convertString(tocTitle));
		painter.DrawText(pdfTocTitle, c_dBorder, currentY);

		currentY -= m_dLineHeight;

		// Current row starting position on the page
		double currentX = 0.0;

		PdfFont* pBoldFont = createFont(true, false);
		for(quint32 i = 0; i < rList.getItemsCount(); ++i)
		{
			const GoShoppingListItem& rItem = rList.getItemAt(i);

			QString strTitle;

			double startX = c_dBorder;
			double deltaX = 0.0;
			if(rItem.getType() == GoShoppingListItemType::Category_Header)
			{
				currentY -= m_dLineHeight / 3;
				painter.TextState.SetFont(*pBoldFont, c_dFontSize);

				strTitle = rItem.getName();
			}
			else
			{
				startX += m_dIndentLength;
				painter.TextState.SetFont(*m_pTextFont, c_dFontSize);

				painter.DrawImage(*imgSquare,
								  currentX + startX,
								  currentY - 0.2 * m_dIndentLength,
								  scaleSquare,
								  scaleSquare);
				deltaX += imgSquare->GetWidth() * scaleSquare;

				strTitle = formatItem(rItem);
			}

			PdfString pdfTitle(convertString(strTitle));

			painter.DrawText(pdfTitle, currentX + startX + deltaX, currentY);

			double titleLength = getLength(m_pTextFont, pdfTitle) + deltaX;
			addItemAdditionalText(rItem, &pPage, painter, startX, currentX, currentY, titleLength);

			currentY -= 2 * m_dLineHeight / 3;

			addPageIfNeeded(&pPage, painter, currentX, currentY);
		}

		painter.FinishDrawing();

		m_spDocument->Save(std::string(strFilename.toUtf8()));
	}
	catch(PdfError& eCode) 
	{
		eCode.PrintErrorMsg();
		return false;
	}

	return true;
}

void ShoppingListExporterPodofo::addItemAdditionalText(const GoShoppingListItem& rItem, 
													   PdfPage** pPage,
													   PdfPainter& rPainter, 
													   double startX, 
													   double& currentX,
													   double& currentY, 
													   double titleLength)
{
	rPainter.GraphicsState.SetFillColor(PdfColor(0.5));

	if(rItem.getCombinedItemsCount() == 1)
	{
		QString addText;
		if(rItem.getSize(0) != Size::Normal)
		{
			addText += m_rConverter.convertSize(rItem.getSize(0), rItem.getAmount(0).getUnit());
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
			PdfString strAddText(convertString(text));

			double addTextLength = getLength(m_pTextFont, strAddText);

			if(titleLength + addTextLength < rPainter.GetCanvas()->GetRectRaw().Width / 2.0 - startX)
			{
				rPainter.DrawText(strAddText, currentX + startX + titleLength, currentY);
			}
			else
			{
				rPainter.DrawText(strAddText, currentX + startX + 1.5 * m_dIndentLength, currentY - 2 * m_dLineHeight / 3);
				currentY -= 2 * m_dLineHeight / 3;

				addPageIfNeeded(pPage, rPainter, currentX, currentY);
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
				strText += m_rConverter.convertSize(rItem.getSize(i), rItem.getAmount(i).getUnit());
			}

			if(rItem.isOptional(i))
			{
				if(!strText.isEmpty())
				{
					strText += ", ";
				}
				strText += tr("Optional");
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
				QString strAmount = m_rConverter.formatAmount(rItem.getAmount(i));
				strText = QString(tr("- %1 of those %2")).arg(strAmount).arg(strText);
			}
			else if(rItem.getAmount(i).getUnit() == Unit::Unitless)
			{
				strText = QString(tr("<li>Unitless amount for \"%1\"</li>")).arg(rItem.getRecipeInfos()[i].m_RecipeName);
			}

			if(!strText.isEmpty())
			{
				PdfString strAddText(convertString(strText));
				rPainter.DrawText(strAddText, currentX + startX + 1.5 * m_dIndentLength, currentY - 2 * m_dLineHeight / 3);
				currentY -= 2 * m_dLineHeight / 3;

				addPageIfNeeded(pPage, rPainter, currentX, currentY);
			}
		}
	}

	rPainter.GraphicsState.SetFillColor(PdfColor(0.0));
}

void ShoppingListExporterPodofo::addPageIfNeeded(PdfPage** pPage, PdfPainter& rPainter, double& currentX, double& rCurrentY)
{
	if(rCurrentY <= c_dBorder)
	{
		if(currentX > 0.0)
		{
			// Add new page

			rPainter.FinishDrawing();

			*pPage = &m_spDocument->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4));
			if(pPage == nullptr)
			{
				throw QException();
			}

			rPainter.SetCanvas(**pPage);

			currentX = 0.0;
		}
		else
		{
			// Second column
			currentX = (*pPage)->GetRectRaw().Width / 2.0;
		}

		rCurrentY = (*pPage)->GetRectRaw().Height - c_dBorder;
	}
}

QString ShoppingListExporterPodofo::formatItem(const GoShoppingListItem& rItem) const
{
	if(rItem.getAmount().size() == 1)
	{
		QString strAmount = m_rConverter.formatAmount(rItem.getAmount().at(0));
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
			strText += m_rConverter.formatAmount(amount, false);
		}
		if(!strText.isEmpty())
		{
			strText = " " + strText;
		}
		return strText + " " + rItem.getName();
	}

	return "";
}

PdfFont* ShoppingListExporterPodofo::createFont(bool bBold, bool bItalic)
{
	// TODO: Needed?
	//PdfFontCreateParams createParams;
	//createParams.Encoding = PdfEncodingFactory::CreateWinAnsiEncoding();
	PdfFont* pFont = &m_spDocument->GetFonts().GetStandard14Font(getFontType(bBold, bItalic));
	if(pFont == nullptr)
	{
		throw QException();
	}
	
	return pFont;
}

PdfString ShoppingListExporterPodofo::convertString(QString strString)
{
	// TODO: encoding?
	return PdfString(strString.toLocal8Bit());
}
