#include "RecipeBookExporterLatex.h"
#include <QTextStream>
#include <QException>
#include <data/RecipeBookConfiguration.h>
#include <data/RecipeBookConfigItem.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
#include <data/AlternativesType.h>
#include "UIStringConverter.h"
#include "RBLatexExporter.h"

using namespace recipebook;

namespace
{
	QString escapeString(QString text)
	{
		text.replace("\\", "\\textbackslash");

		text.replace("\n", "\\\\\n"); 
		
		text.replace("&", "\\&");
		text.replace("%", "\\%");
		text.replace("$", "\\$");
		text.replace("#", "\\#");
		text.replace("_", "\\_");
		text.replace("{", "\\{");
		text.replace("}", "\\}");
		
		text.replace("~", "\\textasciitilde");
		text.replace("^", "\\textasciicircum");		

		text.replace("ä", "\\\"a");
		text.replace("Ä", "\\\"A");

		text.replace("ö", "\\\"o");
		text.replace("Ö", "\\\"O");

		text.replace("ü", "\\\"u");
		text.replace("Ü", "\\\"U");

		return text;
	}

	QString getTitleTag(quint32 uiLevel)
	{
		switch(uiLevel)
		{
		case 0:
			return "\\chapter";

		case 1:
			return "\\section";

		case 2:
			return "\\subsection";

		case 3:
		default:
			return "\\subsubsection";
		}
	}
}

RecipeBookExporterLatex::RecipeBookExporterLatex(const UIStringConverter& rConverter)
:	m_rConverter(rConverter)
{
}

void RecipeBookExporterLatex::exportRecipeBook(QString strFilename, const RBDialogInterface& rDlgInterface)
{
	if(m_Latex.isEmpty())
	{
		// GenerateLatex not called before this?
		throw QException();
	}

	// The exporter deletes itself after generation is complete
	RBLatexExporter* pExporter = new RBLatexExporter();
	pExporter->generatePdf(m_Latex, strFilename, rDlgInterface, 2);

	m_Latex.clear();
}

void RecipeBookExporterLatex::generateLatex(const RecipeBookConfiguration& rConfig)
{
	m_Latex.clear();
	QTextStream stream(&m_Latex);

	stream << "\\documentclass[a4paper,oneside," << rConfig.getFontSize() << "pt]{extbook}\n";
	stream << "\\usepackage[utf8]{inputenc}\n";
	stream << "\\usepackage{a4wide}\n";
	stream << "\\usepackage{amssymb}\n";
	stream << "\\usepackage{multicol}\n";
	stream << "\\usepackage{enumitem}\n";
	stream << "\\usepackage[clock]{ifsym}\n";
	stream << "\\usepackage{fontawesome}\n";
	stream << "\\usepackage{vwcol}\n";

	stream << "\\usepackage{hyperref}\n";
	stream << "\\hypersetup{colorlinks, citecolor = black, filecolor = black, linkcolor = black, urlcolor = black}\n";

	stream << "\\setlength{\\parindent}{0em}\n";

	stream << "\\setlength{\\columnseprule}{1pt}\n";
	stream << "\\setlength{\\columnsep}{3em}\n";
	
	stream << "\\linespread {1.25}\\selectfont\n";

	stream << "\\title{" << escapeString(rConfig.getBookTitle()) << "}\n";
	stream << "\\author{" << escapeString(rConfig.getBookSubtitle()) << "}\n";

	stream << "\\pagestyle{plain}\n";
	stream << "\\begin{document}\n";

	stream << "\\maketitle\n";

	stream << "\\tableofcontents\n";
	stream << "\\newpage\n";

	quint32 uiCurrentLevel = 0;
	for(quint32 i = 0; i < rConfig.getItemsCount(); ++i)
	{
		const RecipeBookConfigItem& rItem = rConfig.getItemAt(i);

		switch(rItem.getType())
		{
			case RecipeBookConfigItemType::Header:
			{
				stream << getTitleTag(rItem.getLevel()) << "{" << rItem.getName() << "}\n";

				uiCurrentLevel = rItem.getLevel() + 1;
				break;
			}

			case RecipeBookConfigItemType::Recipe:
			{
				stream << "\n\\pagebreak\n\n";
				addRecipe(stream, uiCurrentLevel, *rItem.getRecipe());
				break;
			}

			default:
			{
				throw QException();
			}
		}
	}
	
	stream << "\\end{document}\n";
}

void RecipeBookExporterLatex::addRecipe(QTextStream& rStream, quint32 uiCurrentLevel, const Recipe& rRecipe) const
{
	rStream << getTitleTag(uiCurrentLevel) << "{" << rRecipe.getName() << "}\n";

	QString strShortDesc = escapeString(rRecipe.getShortDescription());
	
	QString box = tr("\\faGroup\\, %1").arg(rRecipe.getNumberOfPersons());
	if(rRecipe.getCookingTime().isValid())
	{
		QString duration;
		if(rRecipe.getCookingTime().hour() > 0)
		{
			duration = rRecipe.getCookingTime().toString("HH:mm") + tr("h");
		}
		else
		{
			duration = rRecipe.getCookingTime().toString("mm") + tr("min");
		}
		box += tr("\\,\\, \\Interval\\, %1\n\n").arg(duration);
	}

	if(strShortDesc.isEmpty())
	{
		rStream << "\\hfill" << box;
	}
	else
	{
		rStream << "\\vspace{0.5cm}\n\n";

		rStream << "\\begin{vwcol} [widths = {0.75,0.25}, sep = .8cm, justify = flush, rule = 0pt, indent = 1em]\n";
		rStream << strShortDesc;
		rStream << "\\newpage\\hfill" << box;
		rStream << "\\end{vwcol}\n";
	}
	
	rStream << "\\vspace{0.5cm}\n\n";
	rStream << "{\\hfil\\rule{0.75\\textwidth}{0.4pt}\\hfil\n\n";
	rStream << "\\vspace{0.5cm}\n\n";

	if(rRecipe.getRecipeText().isEmpty())
	{
		// We only have the recipe items, no text
		rStream << formatItems(rRecipe);
	}
	else
	{
		rStream << "\\begin{multicols}{2}\n";
		rStream << formatItems(rRecipe);
		rStream << "\\vfill\n\\vfill\n\n";

		rStream << "\\columnbreak\n";
		
		rStream << escapeString(rRecipe.getRecipeText()) << "\n\\vfill\n";

		rStream << "\\end{multicols}\n";
	}

	rStream << "\\vspace{0.5cm}\n\n";
	rStream << "{\\hfil\\rule{0.75\\textwidth}{0.4pt}\\hfil\n\n";
	rStream << "\\vfill\n";
}

QString RecipeBookExporterLatex::formatItems(const Recipe& rRecipe) const
{
	QString str = "\\begin{itemize}[leftmargin=*,itemsep=1\\itemsep,parsep=1\\parsep,partopsep=1\\partopsep,topsep=0pt]\n";

	QString currentGroup;
	for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
	{
		const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);

		// End of group?
		if(!currentGroup.isEmpty() 
		   && (!rItem.hasAlternativesGroup() || rItem.getAlternativesGroup().getName() != currentGroup))
		{
			// End current group
			str += "\\end{itemize}";
			currentGroup.clear();
		}

		// Begin of new group?
		if(rItem.hasAlternativesGroup() && rItem.getAlternativesGroup().getName() != currentGroup)
		{
			currentGroup = rItem.getAlternativesGroup().getName();

			// Start new group
			str += "\\item \\textbf{" + rItem.getAlternativesGroup().getName() + "}\n";
			str += "\\begin{itemize}[leftmargin=*,itemsep=1\\itemsep,parsep=1\\parsep,partopsep=1\\partopsep,topsep=0pt]\n";
		}

		str += "\\item ";

		if(rItem.isOptional())
		{
			str += "\\textit{";
		}
		else
		{
			str += "\\textbf{";
		}

		QString amount = escapeString(m_rConverter.formatAmount(rItem.getAmount()));
		if(!amount.isEmpty())
		{
			str += amount + " ";
		}

		str += escapeString(rItem.getName());
		str += "}";

		QString addText;
		if(rItem.getSize() != Size::Normal)
		{
			addText += escapeString(m_rConverter.convertSize(rItem.getSize(), rItem.getAmount().getUnit()));
		}
		if(!rItem.getAdditionalInfo().isEmpty())
		{
			if(!addText.isEmpty())
			{
				addText += ", ";
			}
			addText += escapeString(rItem.getAdditionalInfo());
		}
		if(!addText.isEmpty())
		{
			str += " (" + addText + ")";
		}

		str += "\n";
	}

	if(!currentGroup.isEmpty())
	{
		str += "\\end{itemize}\n";
	}

	str += "\\end{itemize}";

	return str;
}
