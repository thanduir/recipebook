#include "RecipeBookExporterLatex.h"
#include <QTextStream>
#include <QException>
#include <data/RecipeBookConfiguration.h>
#include <data/RecipeBookConfigItem.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
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
	// generateLatex needs to be called first
	if(m_Latex.isEmpty())
	{
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

	stream << "\\documentclass[a4paper," << rConfig.getFontSize() << "pt]{extbook}\n";
	stream << "\\usepackage[utf8]{inputenc}\n";
	stream << "\\usepackage{a4wide}\n";
	stream << "\\usepackage{amssymb}\n";
	stream << "\\usepackage{multicol}\n";
	stream << "\\usepackage{enumitem}\n";

	stream << "\\usepackage{hyperref}\n";
	stream << "\\hypersetup{colorlinks, citecolor = black, filecolor = black, linkcolor = black, urlcolor = black}\n";

	stream << "\\setlength{\\parindent}{0em}\n";

	stream << "\\setlength{\\columnseprule}{1pt}\n";

	stream << "\\linespread {1.25}\\selectfont\n";

	stream << "\\title{" << escapeString(rConfig.getBookTitle()) << "}\n";
	stream << "\\author{" << escapeString(rConfig.getBookSubtitle()) << "}\n";

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

void RecipeBookExporterLatex::addRecipe(QTextStream& rStream, quint32 uiCurrentLevel, const Recipe& rItem) const
{
	rStream << getTitleTag(uiCurrentLevel) << "{" << rItem.getName() << "}\n";

	rStream << escapeString(rItem.getShortDescription()) << "\n\n";

	rStream << tr("For %1 persons.\n\n").arg(rItem.getNumberOfPersons());

	if(rItem.getCookingTime().isValid())
	{
		QString duration;
		if(rItem.getCookingTime().hour() > 0)
		{
			duration = rItem.getCookingTime().toString("HH:mm") + tr("h");
		}
		else
		{
			duration = rItem.getCookingTime().toString("mm") + tr("min");
		}
		rStream << tr("Cooking duration: %1.\n\n").arg(duration);
	}

	rStream << "\\vspace{0.5cm}\n\n";

	if(rItem.getRecipeText().isEmpty())
	{
		// We only have the recipe items, no text

		rStream << "\\textbf{\n";
		rStream << "\\begin{itemize}\n";
		// TODO: Items with alternatives group should be grouped together separately in some way!
		for(quint32 i = 0; i < rItem.getRecipeItemsCount(); ++i)
		{
			rStream << "\\item " << formatItem(rItem.getRecipeItemAt(i)) << "\n";
		}
		rStream << "\\end{itemize}\n";
		rStream << "}\\vfill\n";
	}
	else
	{
		rStream << "\\begin{multicols}{2}\n";
		rStream << "\\begin{itemize}[leftmargin=*,itemsep=1\\itemsep,parsep=1\\parsep,partopsep=1\\partopsep,topsep=0pt]\n";
		// TODO: Items with alternatives group should be grouped together separately in some way!
		for(quint32 i = 0; i < rItem.getRecipeItemsCount(); ++i)
		{
			rStream << "\\item " << formatItem(rItem.getRecipeItemAt(i)) << "\n";
		}
		rStream << "\\end{itemize}\\vfill\n";
		rStream << "\\vfill\n\n";

		rStream << "\\columnbreak\n";

		rStream << escapeString(rItem.getRecipeText()) << "\n\\vfill\n";

		rStream << "\\end{multicols}\n\\vfill\n";
	}
}

QString RecipeBookExporterLatex::formatItem(const RecipeItem& rItem) const
{
	QString str;
	if(rItem.isOptional())
	{
		str += "\\textit{";
	}
	else
	{
		str += "\\textbf{";
	}

	str += escapeString(m_rConverter.formatAmount(rItem.getAmount())) + " ";
	if(rItem.getSize() != Size::Normal)
	{
		str += escapeString(m_rConverter.convertSize(rItem.getSize())) + " ";
	}
	str += escapeString(rItem.getName());
	str += "}";

	if(!rItem.getAdditionalInfo().isEmpty())
	{
		str += " (" + escapeString(rItem.getAdditionalInfo()) + ")";
	}
	
	return str;
}
