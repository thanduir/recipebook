#ifndef RECIPEBOOK_CONFIGURATION_EXPORTER_PODOFO_H
#define RECIPEBOOK_CONFIGURATION_EXPORTER_PODOFO_H

#include <memory>
#include <QObject>

#include "podofo.h"

class UIStringConverter;

namespace PoDoFo
{
	class PdfMemDocument;
	class PdfFont;
	class PdfOutlineItem;
	class PdfPainter;
	class PdfString;
	class PdfDestination;
}

namespace recipebook
{
	class RecipeBookConfiguration;
	class Recipe;
	enum class RecipeBookConfigItemType;
	
	class RecipeBookExporterPodofo : public QObject
	{
		Q_OBJECT

	public:
		explicit RecipeBookExporterPodofo(const UIStringConverter& rConverter);
		~RecipeBookExporterPodofo();

		bool writeDocument(const RecipeBookConfiguration& rConfig, QString strFilename);

	private:
		void addTitlePage(QString title, QString subtitle);
		void addChapterHeader(QString strTitle, qint32 level);
		void addRecipePage(const Recipe& rRecipe);
		void addRecipeItems(PoDoFo::PdfPainter& rPainter,
							double minX,
							double currentY,
							const Recipe& rRecipe);
		void addTOC();

		PoDoFo::PdfFont* createFont(bool bBold, bool bItalic = false);
		PoDoFo::PdfString convertString(QString strString);

	private:
		struct TocItem
		{
			QString									m_strName;
			RecipeBookConfigItemType				m_Type;
			qint32									m_Level;
			std::shared_ptr<PoDoFo::PdfDestination>	m_Destination;

			TocItem(QString strName, RecipeBookConfigItemType type, qint32 level, std::shared_ptr<PoDoFo::PdfDestination> destination)
				: m_strName(strName), m_Type(type), m_Level(level), m_Destination(destination) {}
		};

	private:
		const UIStringConverter& m_rConverter;

		std::unique_ptr<PoDoFo::PdfMemDocument> m_spDocument = nullptr;
		
		double									m_FontSize = 0.0;
		PoDoFo::PdfFont*						m_pTextFont = nullptr;
		double									m_dIndentLength = 0.0;
		double									m_dLineHeight = 0.0;

		PoDoFo::PdfOutlineItem*					m_pCurrentParentOutlineItem = nullptr;
		QList<TocItem>							m_TocItems;
		std::shared_ptr<PoDoFo::PdfDestination>	m_spDestinationTitlePage = nullptr;

		std::unique_ptr<PoDoFo::PdfImage>		m_spImgPersons = nullptr;
		std::unique_ptr<PoDoFo::PdfImage>		m_spImgDuration = nullptr;
	};
}

#endif
