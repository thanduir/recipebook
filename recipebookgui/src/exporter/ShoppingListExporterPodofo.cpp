#include "ShoppingListExporterPodofo.h"
#include <QException>
#include <QFile>
#include <data/GoShoppingListItem.h>
#include <data/SortedShoppingList.h>
#include "../uistringconverter.h"

#pragma warning( push )
#pragma warning( disable : 4996 )
#include <podofo/podofo.h>
#pragma warning( pop )

using namespace recipebook;
using namespace PoDoFo;

namespace
{
	const QString c_Author = "RecipeBook";

	const double c_dFontSize = 10;
	const double c_dBorder = 56.69;
	const char* c_DefaultFont = "Calibri";

	double getLength(PdfFont* pFont, PdfString strText)
	{
		return pFont->GetFontMetrics()->StringWidth(strText);
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
		m_spDocument = std::make_unique<PdfStreamedDocument>(strFilename.toUtf8(), ePdfVersion_1_5);

		// Metainformation
		if(m_spDocument->GetInfo() != nullptr)
		{
			m_spDocument->GetInfo()->SetCreator(convertString(c_Author));
			m_spDocument->GetInfo()->SetTitle(convertString(tr("Shopping list")));
		}

		m_dIndentLength = c_dFontSize;
		m_dLineHeight = 1.5 * m_dIndentLength;

		// Default fonts
		m_pTextFont = createFont(c_dFontSize, false, false);
		
		// Images

		auto readImage = [](QString strFilename, PdfImage& rImage)
		{
			QFile f(strFilename);
			f.open(QIODevice::ReadOnly);
			QByteArray ba = f.readAll();
			rImage.LoadFromData(reinterpret_cast<const unsigned char*>(ba.constData()), ba.length());
		};

		PdfImage imgSquare(m_spDocument.get());
		readImage(":/export-images/square.png", imgSquare);
		float scaleSquare = m_dIndentLength / imgSquare.GetHeight();

		// Pages
		
		int currentPageIndex = 1;
		PdfPage* pPage = m_spDocument->InsertPage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4), currentPageIndex);
		if(pPage == nullptr)
		{
			throw QException();
		}

		double currentY = pPage->GetPageSize().GetHeight() - c_dBorder;

		PdfPainter painter;
		painter.SetPage(pPage);

		// Header
		PdfFont* pTitleFont = createFont(4.0 * m_pTextFont->GetFontSize() / 3.0, true, false);
		painter.SetFont(pTitleFont);

		QString tocTitle = tr("Shopping list");
		PdfString pdfTocTitle(convertString(tocTitle));
		painter.DrawText(c_dBorder, currentY, pdfTocTitle);

		currentY -= m_dLineHeight;

		// Current row starting position on the page
		double currentX = 0.0;

		PdfFont* pBoldFont = createFont(m_pTextFont->GetFontSize(), true, false);
		for(quint32 i = 0; i < rList.getItemsCount(); ++i)
		{
			const GoShoppingListItem& rItem = rList.getItemAt(i);

			QString strTitle;

			double startX = c_dBorder;
			double deltaX = 0.0;
			if(rItem.getType() == GoShoppingListItemType::Category_Header)
			{
				currentY -= m_dLineHeight / 3;
				painter.SetFont(pBoldFont);

				strTitle = rItem.getName();
			}
			else
			{
				startX += m_dIndentLength;
				painter.SetFont(m_pTextFont);

				painter.DrawImage(currentX + startX,
								  currentY - 0.2 * m_dIndentLength,
								  &imgSquare,
								  scaleSquare,
								  scaleSquare);
				deltaX += imgSquare.GetWidth() * scaleSquare;

				strTitle = formatItem(rItem);
			}

			PdfString pdfTitle(convertString(strTitle));

			painter.DrawText(currentX + startX + deltaX, currentY, pdfTitle);

			double titleLength = getLength(m_pTextFont, pdfTitle) + deltaX;
			addItemAdditionalText(rItem, pPage, painter, startX, currentX, currentY, titleLength);

			currentY -= 2 * m_dLineHeight / 3;

			addPageIfNeeded(pPage, painter, currentX, currentY);
		}

		painter.FinishPage();

		m_spDocument->Close();
	}
	catch(PdfError& eCode) 
	{
		eCode.PrintErrorMsg();
		return false;
	}

	return true;
}

void ShoppingListExporterPodofo::addItemAdditionalText(const GoShoppingListItem& rItem, 
													   PdfPage*& pPage,
													   PdfPainter& rPainter, 
													   double startX, 
													   double& currentX,
													   double& currentY, 
													   double titleLength)
{
	rPainter.SetGray(0.5);

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

			if(titleLength + addTextLength < rPainter.GetPage()->GetPageSize().GetWidth() / 2.0 - startX)
			{
				rPainter.DrawText(currentX + startX + titleLength, currentY, strAddText);
			}
			else
			{
				rPainter.DrawText(currentX + startX + 1.5 * m_dIndentLength, currentY - 2 * m_dLineHeight / 3, strAddText);
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
				strText = tr("- Also needed in a unitless amount");
			}

			if(!strText.isEmpty())
			{
				PdfString strAddText(convertString(strText));
				rPainter.DrawText(currentX + startX + 1.5 * m_dIndentLength, currentY - 2 * m_dLineHeight / 3, strAddText);
				currentY -= 2 * m_dLineHeight / 3;

				addPageIfNeeded(pPage, rPainter, currentX, currentY);
			}
		}
	}

	rPainter.SetGray(0.0);
}

void ShoppingListExporterPodofo::addPageIfNeeded(PdfPage*& pPage, PdfPainter& rPainter, double& currentX, double& rCurrentY)
{
	if(rCurrentY <= c_dBorder)
	{
		if(currentX > 0.0)
		{
			// Add new page

			rPainter.FinishPage();

			pPage = m_spDocument->CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
			if(pPage == nullptr)
			{
				throw QException();
			}

			rPainter.SetPage(pPage);

			currentX = 0.0;
		}
		else
		{
			// Second column
			currentX = pPage->GetPageSize().GetWidth() / 2.0;
		}

		rCurrentY = pPage->GetPageSize().GetHeight() - c_dBorder;
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

PdfFont* ShoppingListExporterPodofo::createFont(float fSize, bool bBold, bool bItalic)
{
	PdfFont* pFont = m_spDocument->CreateFont(c_DefaultFont, 
											  bBold, 
											  bItalic, 
											  false, 
											  PdfEncodingFactory::GlobalWin1250EncodingInstance());

	if(pFont == nullptr)
	{
		throw QException();
	}
	pFont->SetFontSize(fSize);
	return pFont;
}

PdfString ShoppingListExporterPodofo::convertString(QString strString)
{
	return PdfString(strString.toLocal8Bit(), PdfEncodingFactory::GlobalWin1250EncodingInstance());
}
