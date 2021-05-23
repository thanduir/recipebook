#include "RecipeBookExporterPodofo.h"
#include <QException>
#include <data/RecipeBookConfiguration.h>
#include <data/RecipeBookConfigItem.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
#include <data/AlternativesType.h>
#include "../uistringconverter.h"

using namespace recipebook;
using namespace PoDoFo;

namespace
{
	const QString c_Author = "RecipeBook";

	const double c_dBorder = 56.69;
	const char* c_DefaultFont = "Calibri";

	const QChar c_bulletChar(0x2022);

	double getLength(PdfFont* pFont, PdfString strText)
	{
		return pFont->GetFontMetrics()->StringWidth(strText);
	}
}

RecipeBookExporterPodofo::RecipeBookExporterPodofo(const UIStringConverter& rConverter)
	: m_rConverter(rConverter)
{
}

RecipeBookExporterPodofo::~RecipeBookExporterPodofo()
{
}

bool RecipeBookExporterPodofo::writeDocument(const RecipeBookConfiguration& rConfig, QString strFilename)
{
	try 
	{
		m_spDocument = std::make_unique<PdfStreamedDocument>(strFilename.toUtf8(), ePdfVersion_1_5);

		// Metainformation
		if(m_spDocument->GetInfo() != nullptr)
		{
			m_spDocument->GetInfo()->SetCreator(convertString(c_Author));
			m_spDocument->GetInfo()->SetAuthor(convertString(rConfig.getBookSubtitle()));
			m_spDocument->GetInfo()->SetTitle(convertString(rConfig.getBookTitle()));
		}

		m_dIndentLength = rConfig.getFontSize();
		m_dLineHeight = 2 * m_dIndentLength;

		// Default fonts
		m_pTextFont = createFont(rConfig.getFontSize(), false, false);
		
		PdfOutlineItem* pOutlineRoot = m_spDocument->GetOutlines()->CreateRoot(convertString(rConfig.getBookTitle()));
		m_pCurrentParentOutlineItem = pOutlineRoot;

		// Images
		// TODO: Can i load the pngs from qt resources?

		m_spImgPersons = std::make_unique<PdfImage>(m_spDocument.get());
		m_spImgPersons->LoadFromPng("pdf-pngs/persons.png");

		m_spImgDuration = std::make_unique<PdfImage>(m_spDocument.get());
		m_spImgDuration->LoadFromPng("pdf-pngs/duration.png");

		// Pages
		
		addTitlePage(rConfig.getBookTitle(), rConfig.getBookSubtitle());

		for(quint32 i = 0; i < rConfig.getItemsCount(); ++i)
		{
			const RecipeBookConfigItem& rItem = rConfig.getItemAt(i);

			switch(rItem.getType())
			{
			case RecipeBookConfigItemType::Header:
			{
				if(rItem.getLevel() == 0)
				{
					m_pCurrentParentOutlineItem = pOutlineRoot;
				}

				addChapterHeader(rItem.getName());

				break;
			}

			case RecipeBookConfigItemType::Recipe:
			{
				addRecipePage(*rItem.getRecipe());
				break;
			}

			default:
			{
				throw QException();
			}
			}
		}

		addTOC();

		m_spDocument->Close();
	}
	catch(PdfError& eCode) 
	{
		eCode.PrintErrorMsg();
		return false;
	}

	return true;
}

void RecipeBookExporterPodofo::addTitlePage(QString title, QString subtitle)
{
	PdfPage* pPage = m_spDocument->CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
	if(pPage == nullptr)
	{
		throw QException();
	}

	PdfFont* pFont = createFont(m_pTextFont->GetFontSize() * 2, true, false);

	m_pCurrentParentOutlineItem->SetDestination(pPage);

	PdfPainter painter;
	painter.SetPage(pPage);
	painter.SetFont(pFont);

	// Page layout

	PdfString pdfTitle(convertString(title));
	painter.DrawText(pPage->GetPageSize().GetWidth() / 2.0 - getLength(pFont, pdfTitle) / 2, pPage->GetPageSize().GetHeight() / 2.0 + c_dBorder, pdfTitle);

	pFont->SetFontSize(pFont->GetFontSize() * 0.8);
	PdfString pdfSubTitle(convertString(subtitle));
	painter.DrawText(pPage->GetPageSize().GetWidth() / 2.0 - getLength(pFont, pdfSubTitle) / 2, pPage->GetPageSize().GetHeight() / 2.0, pdfSubTitle);

	painter.FinishPage();
}

void RecipeBookExporterPodofo::addChapterHeader(QString strTitle)
{
	PdfPage* pPage = m_spDocument->CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
	if(pPage == nullptr)
	{
		throw QException();
	}

	PdfPainter painter;
	painter.SetPage(pPage);
	PdfFont* pTitleFont = createFont(4.0 * m_pTextFont->GetFontSize() / 3.0, true, false);
	painter.SetFont(pTitleFont);

	// Define page

	PdfString pdfTitle(convertString(strTitle));
	painter.DrawText(c_dBorder, pPage->GetPageSize().GetHeight() - 2.0 * c_dBorder, pdfTitle);

	painter.FinishPage();

	PdfDestination outlineDest(pPage);
	m_pCurrentParentOutlineItem = m_pCurrentParentOutlineItem->CreateChild(pdfTitle, outlineDest);
	m_TocItems.append(TocItem(strTitle, RecipeBookConfigItemType::Header, outlineDest));
}

void RecipeBookExporterPodofo::addRecipePage(const Recipe& rRecipe)
{
	PdfPage* pPage = m_spDocument->CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
	if(pPage == nullptr)
	{
		throw QException();
	}

	PdfPainter painter;
	painter.SetPage(pPage);

	// Define page

	double currentY = pPage->GetPageSize().GetHeight() - 2.0 * c_dBorder;

	// Header
	PdfFont* pTitleFont = createFont(4.0 * m_pTextFont->GetFontSize() / 3.0, true, false);
	painter.SetFont(pTitleFont);
	PdfString pdfTitle(convertString(rRecipe.getName()));
	painter.DrawText(c_dBorder, currentY, pdfTitle);

	currentY -= m_dLineHeight * 2;
	painter.SetFont(m_pTextFont);

	// Short desc, number of persons and duration

	PdfString pdfShortDesc(convertString(rRecipe.getShortDescription()));

	// Number of persons and duration
	QString strDuration;
	if(rRecipe.getCookingTime().isValid())
	{
		if(rRecipe.getCookingTime().hour() > 0)
		{
			strDuration = rRecipe.getCookingTime().toString("HH:mm") + QObject::tr("h");
		}
		else
		{
			strDuration = rRecipe.getCookingTime().toString("mm") + QObject::tr("min");
		}
	}

	float scalePersons = 1.1 * m_dIndentLength / m_spImgPersons->GetHeight();
	QString strTextPersons = QString("%1").arg(rRecipe.getNumberOfPersons());
	PdfString pdfTextPersons(convertString(strTextPersons));

	float scaleDuration = 1.1 * m_dIndentLength / m_spImgDuration->GetHeight();
	QString strTextDuration = QString("%1").arg(strDuration);
	PdfString pdfTextDuration(convertString(strTextDuration));

	double topRightLength = m_spImgDuration->GetWidth() * scaleDuration + getLength(m_pTextFont, pdfTextDuration);
	double twoRowHeight = 1.5 * m_dLineHeight;

	// Short desc
	painter.DrawMultiLineText(c_dBorder,
							  currentY - twoRowHeight / 2.0,
							  pPage->GetPageSize().GetWidth() - 2.0 * c_dBorder - topRightLength,
							  twoRowHeight,
							  pdfShortDesc);

	// Nr persons
	double startX = pPage->GetPageSize().GetWidth() - c_dBorder - topRightLength;
	double startY = currentY + 0.5 * m_dIndentLength;
	painter.DrawImage(startX,
					  startY,
					  m_spImgPersons.get(),
					  scalePersons, 
					  scalePersons);
	painter.DrawText(startX + m_spImgPersons->GetWidth() * scalePersons + m_dIndentLength * 0.5,
					 startY + 0.25 * m_dIndentLength,
					 pdfTextPersons);

	// Duration
	double yDuration = startY - 1.25 * m_spImgPersons->GetHeight() * scalePersons;
	painter.DrawImage(startX,
					  yDuration,
					  m_spImgDuration.get(),
					  scaleDuration,
					  scaleDuration);
	painter.DrawText(startX + m_spImgDuration->GetWidth() * scaleDuration + m_dIndentLength * 0.5,
					 yDuration + 0.25 * m_dIndentLength,
					 pdfTextDuration);
		
	currentY -= m_dLineHeight;

	// Horizontal line
	painter.DrawLine(pPage->GetPageSize().GetWidth() / 6.0,
					 currentY,
					 5 * pPage->GetPageSize().GetWidth() / 6.0,
					 currentY);
	currentY -= m_dLineHeight;

	// Items
	addRecipeItems(painter, c_dBorder, currentY - 12, rRecipe);

	// Middle line
	painter.DrawLine(pPage->GetPageSize().GetWidth() / 2.0,
					 currentY,
					 pPage->GetPageSize().GetWidth() / 2.0,
					 c_dBorder);


	PdfString pdfText(convertString(rRecipe.getRecipeText()));
	double textHeight = currentY - c_dBorder;
	painter.DrawMultiLineText(pPage->GetPageSize().GetWidth() / 2.0 + m_dIndentLength,
							  c_dBorder,
							  pPage->GetPageSize().GetWidth() / 2.0 - m_dIndentLength - c_dBorder,
							  textHeight,
							  pdfText);

	painter.FinishPage();

	PdfDestination outlineDest(pPage, ePdfDestinationFit_FitB);
	m_pCurrentParentOutlineItem->CreateChild(pdfTitle, outlineDest);
	m_TocItems.append(TocItem(rRecipe.getName(), RecipeBookConfigItemType::Recipe, outlineDest));
}

void RecipeBookExporterPodofo::addRecipeItems(PdfPainter& rPainter,
											  double minX,
											  double currentY,
											  const Recipe& rRecipe)
{
	PdfFont* pBold = createFont(m_pTextFont->GetFontSize(), true, false);
	PdfFont* pItalic = createFont(m_pTextFont->GetFontSize(), false, true);
	PdfFont* pDefault = rPainter.GetFont();

	double x = minX;
	double y = currentY;

	QString currentGroup;
	for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
	{
		const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);

		// End of group?
		if(!currentGroup.isEmpty()
		   && (!rItem.hasAlternativesGroup() || rItem.getAlternativesGroup().getName() != currentGroup))
		{
			// End current group
			x -= m_dIndentLength;
			currentGroup.clear();
		}

		// Begin of new group?
		if(rItem.hasAlternativesGroup() && rItem.getAlternativesGroup().getName() != currentGroup)
		{
			currentGroup = rItem.getAlternativesGroup().getName();

			rPainter.SetFont(pBold);
			QString strGroup = QString("%1 %2").arg(c_bulletChar).arg(rItem.getAlternativesGroup().getName());
			PdfString str(convertString(strGroup));
			rPainter.DrawText(x, y, str);

			x += m_dIndentLength;
			y -= m_dLineHeight;
		}

		QString itemText = !currentGroup.isEmpty() ? "- " : QString("%1 ").arg(c_bulletChar);
		QString amount = m_rConverter.formatAmount(rItem.getAmount());
		if(!amount.isEmpty())
		{
			itemText += amount + " ";
		}
		itemText += rItem.getName();

		PdfString strItemText(convertString(itemText));
		if(rItem.isOptional())
		{
			rPainter.SetFont(pItalic);
			rPainter.DrawText(x, y, strItemText);
		}
		else
		{
			rPainter.SetFont(pBold);
			rPainter.DrawText(x, y, strItemText);
		}

		rPainter.SetFont(pDefault);

		QString addText;
		if(rItem.getSize() != Size::Normal)
		{
			addText += m_rConverter.convertSize(rItem.getSize(), rItem.getAmount().getUnit());
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
			PdfString strAddText(convertString(text));

			double itemTextLength = getLength(rItem.isOptional() ? pItalic : pBold, strItemText);
			double addTextLength = getLength(pDefault, strAddText);

			if(itemTextLength + addTextLength < rPainter.GetPage()->GetPageSize().GetWidth() / 2.0 - minX)
			{
				rPainter.DrawText(x + itemTextLength, y, strAddText);
			}
			else
			{
				rPainter.DrawText(x + m_dIndentLength, y - 1.25 * m_dIndentLength, strAddText);
				y -= 1.25 * m_dIndentLength;
			}
		}

		y -= m_dLineHeight;
	}
}

void RecipeBookExporterPodofo::addTOC()
{
	int currentPageIndex = 1;
	PdfPage* pPage = m_spDocument->InsertPage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4), currentPageIndex);
	if(pPage == nullptr)
	{
		throw QException();
	}

	double currentY = pPage->GetPageSize().GetHeight() - 2.0 * c_dBorder;

	PdfPainter painter;
	painter.SetPage(pPage);
	
	// Header
	PdfFont* pTitleFont = createFont(4.0 * m_pTextFont->GetFontSize() / 3.0, true, false);
	painter.SetFont(pTitleFont);

	QString tocTitle = tr("Table of contents");
	PdfString pdfTocTitle(convertString(tocTitle));
	painter.DrawText(c_dBorder, currentY, pdfTocTitle);

	currentY -= m_dLineHeight;

	PdfFont* pBoldFont = createFont(m_pTextFont->GetFontSize(), true, false);
	for(const TocItem& rItem : m_TocItems)
	{
		PdfString pdfTitle(convertString(rItem.m_strName));

		double startX = c_dBorder;
		if(rItem.m_Type == RecipeBookConfigItemType::Recipe)
		{
			startX += m_dIndentLength;
			painter.SetFont(m_pTextFont);
		}
		else
		{
			currentY -= m_dLineHeight / 3;
			painter.SetFont(pBoldFont);
		}

		painter.DrawText(startX, currentY, pdfTitle);

		PdfRect rect(startX, currentY, getLength(painter.GetFont(), pdfTitle), painter.GetFont()->GetFontSize());
		PdfAnnotation* pAnnotation = pPage->CreateAnnotation(ePdfAnnotation_Link, rect);
		pAnnotation->SetDestination(rItem.m_Destination);
		pAnnotation->SetBorderStyle(0, 0, 0);

		currentY -= 2 * m_dLineHeight / 3;

		if(currentY <= c_dBorder)
		{
			// Add new page

			painter.FinishPage();
			++currentPageIndex;

			pPage = m_spDocument->InsertPage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4), currentPageIndex);
			if(pPage == nullptr)
			{
				throw QException();
			}

			painter.SetPage(pPage);

			currentY = pPage->GetPageSize().GetHeight() - 2.0 * c_dBorder;
		}
	}

	painter.FinishPage();
}

PdfFont* RecipeBookExporterPodofo::createFont(float fSize, bool bBold, bool bItalic)
{
	PdfFont* pFont = m_spDocument->CreateFont(c_DefaultFont, 
											  bBold, 
											  bItalic, 
											  false, 
											  /*PdfEncodingFactory::GlobalIdentityEncodingInstance()*/ PdfEncodingFactory::GlobalWin1250EncodingInstance());

	if(pFont == nullptr)
	{
		throw QException();
	}
	pFont->SetFontSize(fSize);
	return pFont;
}

PdfString RecipeBookExporterPodofo::convertString(QString strString)
{
	return PdfString(strString.toLocal8Bit(), PdfEncodingFactory::GlobalWin1250EncodingInstance());
}
