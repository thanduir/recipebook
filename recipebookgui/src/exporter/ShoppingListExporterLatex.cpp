#include "ShoppingListExporterLatex.h"
#include <QTextStream>
#include <QException>
#include <data/SortedShoppingList.h>
#include <data/GoShoppingListItem.h>
#include "../uistringconverter.h"
#include "../RecipeBookSettings.h"
#include "RBLatexExporter.h"
#include "LatexLanguageManager.h"

using namespace recipebook;

ShoppingListExporterLatex::ShoppingListExporterLatex(const UIStringConverter& rConverter, const RecipeBookSettings& rSettings)
:	m_rConverter(rConverter),
	m_rSettings(rSettings)
{
}

void ShoppingListExporterLatex::exportShoppingList(QString strFilename, const SortedShoppingList& rList, const RBDialogInterface& rDlgInterface, QString languageCode)
{
	QString latexCode = generateLatex(rList, languageCode);

	// The exporter deletes itself after generation is complete
	RBLatexExporter* pExporter = new RBLatexExporter(m_rSettings.getPdfLatexFile());
	pExporter->generatePdf(latexCode, strFilename, rDlgInterface);
}

QString ShoppingListExporterLatex::generateLatex(const SortedShoppingList& list, QString languageCode)
{
	QString text;
	QTextStream stream(&text);

	LatexLanguageManager language;

	stream << "\\documentclass[a4paper,9pt]{extarticle}\n";
	stream << "\\usepackage[utf8]{inputenc}\n";
	stream << language.getBabelStringFromLangCode(languageCode);
	stream << "\\usepackage{xcolor}\n";
	stream << "\\usepackage{a4wide}\n";
	stream << "\\usepackage{amssymb}\n";
	stream << "\\usepackage{multicol}\n";
	stream << "\\usepackage{enumitem}\n";

	stream << "\\addtolength{\\oddsidemargin}{-.875in}\n";
	stream << "\\addtolength{\\evensidemargin}{-.875in}\n";
	stream << "\\addtolength{\\textwidth}{1.75in}\n";

	stream << "\\addtolength{\\topmargin}{-.875in}\n";
	stream << "\\addtolength{\\textheight}{1.75in}\n";

	stream << "\\setlength{\\parindent}{0em}\n";

	stream << "\\begin{document}\n";
	
	stream << "\\pagestyle{empty}\n";

	stream << "\\section*{" << tr("Shopping list") << "}\n";

	stream << "\\begin{multicols}{2}\n";

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
					stream << "\\end{itemize}\\\n";
				}

				stream << "\\filbreak\\textbf{" << rItem.getName() << "}\n";
				stream << "\\begin{itemize}[noitemsep,topsep=0pt]\n";
				stream << "\\renewcommand{\\labelitemi}{$\\square$}\n";
				bListOpen = true;
				break;
			}

			case GoShoppingListItemType::IngredientListItem:
			{
				stream << "\\filbreak\\item " << formatItem(rItem);
				stream << " {\\color{gray}" << getItemAdditionalText(rItem) << "}\n";
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
		stream << "\\end{itemize}\n";
	}

	stream << "\\end{multicols}\n";
	stream << "\\end{document}\n";
	
	return text;
}

QString ShoppingListExporterLatex::formatItem(const GoShoppingListItem& rItem) const
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

QString ShoppingListExporterLatex::getItemAdditionalText(const GoShoppingListItem& rItem) const
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
				lines += QString(tr("\\item %1 of those %2\n")).arg(strAmount).arg(strText);
			}
			else if(rItem.getAmount(i).getUnit() == Unit::Unitless)
			{
				lines += tr("\\item Also needed in a unitless amount\n");
			}
				 
		}

		if(!lines.isEmpty())
		{
			lines = "\n\\begin{itemize}[noitemsep,topsep=0pt]\n" + lines + "\\end{itemize}";
		}
		return lines;
	}
	
	return "";
}
