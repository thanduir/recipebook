#include "RecipeBookExporterPodofo.h"
#include <QException>
#include <QFile>
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

	const QChar c_bulletChar(0x2022);

	double getLength(PdfFont* pFont, PdfString strText, double fontSize)
	{
		PdfTextState state;
		state.Font = pFont;
		state.FontSize = fontSize;
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
			return PdfStandard14FontType::HelveticaBold;
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
		m_spDocument = std::make_unique<PdfMemDocument>();
		
		// Metainformation
		if(m_spDocument->GetInfo() != nullptr)
		{
			m_spDocument->GetMetadata().SetCreator(convertString(c_Author));
			m_spDocument->GetMetadata().SetAuthor(convertString(rConfig.getBookSubtitle()));
			m_spDocument->GetMetadata().SetTitle(convertString(rConfig.getBookTitle()));
		}

		m_FontSize = rConfig.getFontSize();

		m_dIndentLength = rConfig.getFontSize();
		m_dLineHeight = 2.5 * m_dIndentLength;

		// Default fonts
		m_pTextFont = createFont(false, false);
		
		PdfOutlineItem* pOutlineRoot = m_spDocument->GetOrCreateOutlines().CreateRoot(convertString(rConfig.getBookTitle()));
		m_pCurrentParentOutlineItem = pOutlineRoot;

		// Images

		auto readImage = [](QString _strFilename, std::unique_ptr<PdfImage>& spImage)
		{
			QFile f(_strFilename);
			f.open(QIODevice::ReadOnly);
			QByteArray ba = f.readAll();
			spImage->LoadFromBuffer({ba.data(), static_cast<std::size_t>(ba.size())});
		};

		m_spImgPersons = m_spDocument->CreateImage();
		readImage(":/export-images/persons.png", m_spImgPersons);
		
		m_spImgDuration = m_spDocument->CreateImage();
		readImage(":/export-images/duration.png", m_spImgDuration);

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

				addChapterHeader(rItem.getName(), rItem.getLevel());

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

		m_spDocument->Save(std::string(strFilename.toUtf8()));
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
	PdfPage& rPage = m_spDocument->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4));

	double fontSize = m_FontSize * 1.6;
	PdfFont* pFont = createFont(true, false);

	auto destination = std::make_shared<PdfDestination>(rPage);
	m_pCurrentParentOutlineItem->SetDestination(destination);

	PdfPainter painter;
	painter.SetCanvas(rPage);
	painter.TextState.SetFont(*pFont, fontSize);
	painter.TextState.SetWordSpacing(-12);	

	// Page layout

	PdfString pdfTitle(convertString(title));
	painter.DrawText(pdfTitle, rPage.GetRect().Width / 2.0 - getLength(pFont, pdfTitle, fontSize) / 2.0, rPage.GetRect().Height / 2.0 + c_dBorder);

	painter.TextState.SetFont(*pFont, 1.3 * m_FontSize);
	PdfString pdfSubTitle(convertString(subtitle));
	painter.DrawText(pdfSubTitle, rPage.GetRect().Width / 2.0 - getLength(pFont, pdfSubTitle, 1.3 * m_FontSize) / 2.0, rPage.GetRect().Height / 2.0);

	painter.FinishDrawing();
	
	m_spDestinationTitlePage = std::make_shared<PdfDestination>(rPage, PdfDestinationFit::Fit);
}

void RecipeBookExporterPodofo::addChapterHeader(QString strTitle, qint32 level)
{
	PdfPage& rPage = m_spDocument->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4));

	// TODO: Adjust to level

	PdfPainter painter;
	painter.SetCanvas(rPage);
	PdfFont* pTitleFont = createFont(true, false);
	painter.TextState.SetFont(*pTitleFont, 4.0 * m_FontSize / 3.0);
	painter.TextState.SetWordSpacing(-12);

	// Define page

	PdfString pdfTitle(convertString(strTitle));
	painter.DrawText(pdfTitle, c_dBorder, rPage.GetRect().Height - 2.0 * c_dBorder);

	painter.FinishDrawing();

	auto outlineDest = std::make_shared<PdfDestination>(rPage);
	m_pCurrentParentOutlineItem = m_pCurrentParentOutlineItem->CreateChild(pdfTitle, outlineDest);
	m_TocItems.append(TocItem(strTitle, RecipeBookConfigItemType::Header, level, outlineDest));
}

void RecipeBookExporterPodofo::addRecipePage(const Recipe& rRecipe)
{
	PdfPage& rPage = m_spDocument->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4));

	PdfPainter painter;
	painter.SetCanvas(rPage);

	// Define page

	double currentY = rPage.GetRect().Height - 2.0 * c_dBorder;

	// Header
	PdfFont* pTitleFont = createFont(true, false);
	double titleFontSize = 4.0 * m_FontSize / 3.0;
	painter.TextState.SetFont(*pTitleFont, titleFontSize);
	painter.TextState.SetWordSpacing(-12);
	PdfString pdfTitle(convertString(rRecipe.getName()));
	painter.DrawText(pdfTitle, c_dBorder, currentY);

	if(m_spDestinationTitlePage != nullptr)
	{
		Rect rect(c_dBorder, currentY, getLength(pTitleFont, pdfTitle, m_FontSize), titleFontSize);
		PdfAnnotation& annotation = rPage.GetAnnotations().CreateAnnot(PdfAnnotationType::Link, rect);
		static_cast<PdfAnnotationLink&>(annotation).SetDestination(m_spDestinationTitlePage);
		annotation.SetBorderStyle(0, 0, 0);
	}

	currentY -= m_dLineHeight * 2;
	painter.TextState.SetFont(*m_pTextFont, m_FontSize);
	painter.TextState.SetWordSpacing(-1);

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

	double scalePersons = 1.1 * m_dIndentLength / m_spImgPersons->GetHeight();
	QString strTextPersons = QString("%1").arg(rRecipe.getNumberOfPersons());
	PdfString pdfTextPersons(convertString(strTextPersons));

	double scaleDuration = 1.1 * m_dIndentLength / m_spImgDuration->GetHeight();
	QString strTextDuration = QString("%1").arg(strDuration);
	PdfString pdfTextDuration(convertString(strTextDuration));

	double topRightLength = m_spImgDuration->GetWidth() * scaleDuration + getLength(m_pTextFont, pdfTextDuration, m_FontSize);
	double twoRowHeight = 1.5 * m_dLineHeight;

	PdfDrawTextMultiLineParams multilineParams;
	multilineParams.SkipSpaces = false;

	// Short desc
	painter.DrawTextMultiLine(pdfShortDesc,
							  c_dBorder,
							  currentY - twoRowHeight / 2.0,
							  rPage.GetRect().Width - 2.0 * c_dBorder - topRightLength,
							  twoRowHeight,
							  multilineParams);

	// Nr persons
	double startX = rPage.GetRect().Width - c_dBorder - topRightLength;
	double startY = currentY + 0.5 * m_dIndentLength;
	painter.DrawImage(*m_spImgPersons,
					  startX,
					  startY,
					  scalePersons, 
					  scalePersons);
	painter.DrawText(pdfTextPersons,
					 startX + m_spImgPersons->GetWidth() * scalePersons + m_dIndentLength * 0.5,
					 startY + 0.25 * m_dIndentLength);

	// Duration
	double yDuration = startY - 1.25 * m_spImgPersons->GetHeight() * scalePersons;
	painter.DrawImage(*m_spImgDuration,
					  startX,
					  yDuration,
					  scaleDuration,
					  scaleDuration);
	painter.DrawText(pdfTextDuration,
					 startX + m_spImgDuration->GetWidth() * scaleDuration + m_dIndentLength * 0.5,
					 yDuration + 0.25 * m_dIndentLength);
		
	currentY -= m_dLineHeight;

	// Horizontal line
	painter.DrawLine(rPage.GetRect().Width / 6.0,
					 currentY,
					 5 * rPage.GetRect().Width / 6.0,
					 currentY);
	currentY -= m_dLineHeight;

	// Items
	addRecipeItems(painter, c_dBorder, currentY - 12, rRecipe);

	// Middle line
	painter.DrawLine(rPage.GetRect().Width / 2.0,
					 currentY,
					 rPage.GetRect().Width / 2.0,
					 c_dBorder);


	PdfString pdfText(convertString(rRecipe.getRecipeText()));
	double textHeight = currentY - c_dBorder;

	painter.DrawTextMultiLine(pdfText,
							  rPage.GetRect().Width / 2.0 + m_dIndentLength,
							  c_dBorder,
							  rPage.GetRect().Width / 2.0 - m_dIndentLength - c_dBorder,
							  textHeight,
							  multilineParams);

	painter.FinishDrawing();

	auto outlineDest = std::make_shared<PdfDestination>(rPage, PdfDestinationFit::FitB);
	m_pCurrentParentOutlineItem->CreateChild(pdfTitle, outlineDest);
	m_TocItems.append(TocItem(rRecipe.getName(), RecipeBookConfigItemType::Recipe, -1, outlineDest));
}

void RecipeBookExporterPodofo::addRecipeItems(PdfPainter& rPainter,
											  double minX,
											  double currentY,
											  const Recipe& rRecipe)
{
	PdfFont* pBold = createFont(true, false);
	PdfFont* pItalic = createFont(false, true);
	PdfFont* pDefault = m_pTextFont;

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

			rPainter.TextState.SetFont(*pBold, m_FontSize);
			rPainter.TextState.SetWordSpacing(-9);
			QString strGroup = QString("%1 %2").arg(c_bulletChar).arg(rItem.getAlternativesGroup().getName());
			PdfString str(convertString(strGroup));
			rPainter.DrawText(str, x, y);

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
			rPainter.TextState.SetFont(*pItalic, m_FontSize);
			rPainter.TextState.SetWordSpacing(-1);
			rPainter.DrawText(strItemText, x, y);
		}
		else
		{
			rPainter.TextState.SetFont(*pBold, m_FontSize);
			rPainter.TextState.SetWordSpacing(-9);
			rPainter.DrawText(strItemText, x, y);
		}

		rPainter.TextState.SetFont(*pDefault, m_FontSize);
		rPainter.TextState.SetWordSpacing(-1);

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

			double itemTextLength = getLength(rItem.isOptional() ? pItalic : pBold, strItemText, m_FontSize);
			double addTextLength = getLength(pDefault, strAddText, m_FontSize);

			if(itemTextLength + addTextLength < rPainter.GetCanvas()->GetRectRaw().Width / 2.0 - minX)
			{
				rPainter.DrawText(strAddText, x + itemTextLength, y);
			}
			else
			{
				rPainter.DrawText(strAddText, x + m_dIndentLength, y - 1.25 * m_dIndentLength);
				y -= 1.25 * m_dIndentLength;
			}
		}

		y -= m_dLineHeight;
	}
}

void RecipeBookExporterPodofo::addTOC()
{
	unsigned int currentPageIndex = 1;
	PdfPage* pPage = &m_spDocument->GetPages().CreatePageAt(currentPageIndex, PdfPage::CreateStandardPageSize(PdfPageSize::A4));

	double currentY = pPage->GetRect().Height - 2.0 * c_dBorder;

	PdfPainter painter;
	painter.SetCanvas(*pPage);
	
	// Header
	PdfFont* pTitleFont = createFont(true, false);
	painter.TextState.SetFont(*pTitleFont, 4.0 * m_FontSize / 3.0);
	painter.TextState.SetWordSpacing(-12);

	QString tocTitle = tr("Table of contents");
	PdfString pdfTocTitle(convertString(tocTitle));
	painter.DrawText(pdfTocTitle, c_dBorder, currentY);

	currentY -= m_dLineHeight;

	qint32 currentLevel = 0;
	PdfFont* pBoldFont = createFont(true, false);
	for(const TocItem& rItem : m_TocItems)
	{
		PdfString pdfTitle(convertString(rItem.m_strName));

		double startX = c_dBorder;
		if(rItem.m_Type == RecipeBookConfigItemType::Recipe)
		{
			startX += m_dIndentLength;
			painter.TextState.SetFont(*m_pTextFont, m_FontSize);
			painter.TextState.SetWordSpacing(-1);
		}
		else
		{
			currentLevel = rItem.m_Level;
			
			if(currentLevel == 0)
			{
				currentY -= m_dLineHeight / 3;
			}
			painter.TextState.SetFont(*pBoldFont, m_FontSize);
			painter.TextState.SetWordSpacing(-9);
		}

		startX += currentLevel * m_dIndentLength;

		painter.DrawText(pdfTitle, startX, currentY);

		Rect rect(startX, currentY, getLength(m_pTextFont, pdfTitle, m_FontSize), m_FontSize);
		PdfAnnotation& annotation = pPage->GetAnnotations().CreateAnnot(PdfAnnotationType::Link, rect);
		static_cast<PdfAnnotationLink&>(annotation).SetDestination(rItem.m_Destination);
		annotation.SetBorderStyle(0, 0, 0);

		currentY -= 2 * m_dLineHeight / 3;

		if(currentY <= c_dBorder)
		{
			// Add new page

			painter.FinishDrawing();
			++currentPageIndex;

			pPage = &m_spDocument->GetPages().CreatePageAt(currentPageIndex, PdfPage::CreateStandardPageSize(PdfPageSize::A4));

			painter.SetCanvas(*pPage);

			currentY = pPage->GetRect().Height - 2.0 * c_dBorder;
		}
	}

	painter.FinishDrawing();
}

PdfFont* RecipeBookExporterPodofo::createFont(bool bBold, bool bItalic)
{
	PdfFontCreateParams createParams;
	createParams.Encoding = PdfEncodingFactory::CreateWinAnsiEncoding();
	PdfFont* pFont = &m_spDocument->GetFonts().GetStandard14Font(getFontType(bBold, bItalic), createParams);
	if(pFont == nullptr)
	{
		throw QException();
	}

	return pFont;
}

PdfString RecipeBookExporterPodofo::convertString(QString strString)
{
	return PdfString(strString.replace("\n\n", "\n \n").toLocal8Bit());
}
