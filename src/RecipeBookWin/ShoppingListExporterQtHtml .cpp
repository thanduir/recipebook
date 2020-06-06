#include "ShoppingListExporterQtHtml.h"
#include <QFont>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <QException>
#include "UIStringConverter.h"
#include <data/SortedShoppingList.h>
#include <data/GoShoppingListItem.h>

using namespace recipebook;

ShoppingListExporterQtHtml::ShoppingListExporterQtHtml(const UIStringConverter& rConverter)
:	m_rConverter(rConverter)
{
}

void ShoppingListExporterQtHtml::exportShoppingList(QString strFilename, const SortedShoppingList& rList)
{
	QString strHTMLText = generateHTML(rList);
	exportPdf(strFilename, strHTMLText);
}

void ShoppingListExporterQtHtml::exportPdf(QString strFilename, QString strFormattedText)
{
	QPrinter printer(QPrinter::PrinterResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setPaperSize(QPrinter::A4);
	printer.setOutputFileName(strFilename);
	printer.setPageMargins(QMarginsF(30, 30, 30, 30));

	QTextDocument document;
	document.setIndentWidth(20);
	document.setHtml(strFormattedText);

	QFont defaultFont;
	defaultFont.setFamily("Courier");
	defaultFont.setPointSize(8);

	document.setDefaultFont(defaultFont);

	document.setPageSize(printer.pageRect().size());
	document.print(&printer);
}

QString ShoppingListExporterQtHtml::generateHTML(const SortedShoppingList& list)
{
	QString text;
	QTextStream stream(&text);

	stream << "<h2>Shopping list</h2>";

	bool bListOpen = false;
	for(quint32 i = 0; i < list.getItemsCount(); ++i)
	{
		const GoShoppingListItem& rItem = list.getItemAt(i);

		switch(rItem.getType())
		{
			case GoShoppingListItemType::Category_Header:
			{
				if(bListOpen)
				{
					stream << "</ul>";
				}
				stream << "<h3>" << rItem.getName() << "</h3>";
				stream << "<ul style=\"list-style-type:circle\">";
				bListOpen = true;
				break;
			}

			case GoShoppingListItemType::IngredientListItem:
			{
				stream << "<li>" << formatItem(rItem);
				stream << " <font color=\"gray\">" << getItemAdditionalText(rItem) << "</font>";
				stream << "</li>";
				break;
			}

			default:
			{
				throw QException();
			}
		}
	}

	if(bListOpen)
	{
		stream << "</ul>";
	}

	return text;
}

QString ShoppingListExporterQtHtml::formatItem(const GoShoppingListItem& rItem) const
{
	if(rItem.getAmount().size() == 1)
	{
		return m_rConverter.formatAmount(rItem.getAmount().at(0)) + " " + rItem.getName();
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
		return strText + " " + rItem.getName();
	}

	return "";
}

QString ShoppingListExporterQtHtml::getItemAdditionalText(const GoShoppingListItem& rItem) const
{
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
			addText = "(" + addText + ")";
		}

		return addText;
	}
	else if(rItem.getCombinedItemsCount() > 1)
	{
		QString lines;
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
				lines += QString(tr("<li>- %1 of those %2</li>")).arg(strAmount).arg(strText);
			}
			else if(rItem.getAmount(i).getUnit() == Unit::Unitless)
			{
				lines += "<li>- Also needed in a unitless amount</li>";
			}
				 
		}

		if(!lines.isEmpty())
		{
			lines = "<ul style=\"list-style-type:none\">" + lines + "</ul>";
		}
		return lines;
	}
	
	return "";
}
