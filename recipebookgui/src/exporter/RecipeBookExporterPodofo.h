#ifndef RECIPEBOOK_CONFIGURATION_EXPORTER_PODOFO_H
#define RECIPEBOOK_CONFIGURATION_EXPORTER_PODOFO_H

#include <memory>
#include <QObject>

#pragma warning( push )
#pragma warning( disable : 4996 )
#include <podofo/podofo.h>
#pragma warning( pop )

class UIStringConverter;

namespace PoDoFo
{
	class PdfStreamedDocument;
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

		PoDoFo::PdfFont* createFont(float fSize, bool bBold, bool bItalic = false);
		PoDoFo::PdfString convertString(QString strString);

	private:
		struct TocItem
		{
			QString						m_strName;
			RecipeBookConfigItemType	m_Type;
			qint32						m_Level;
			PoDoFo::PdfDestination		m_Destination;

			TocItem(QString strName, RecipeBookConfigItemType type, qint32 level, PoDoFo::PdfDestination& rDestination)
				: m_strName(strName), m_Type(type), m_Level(level), m_Destination(rDestination) {}
		};

	private:
		const UIStringConverter& m_rConverter;

		std::unique_ptr<PoDoFo::PdfStreamedDocument> m_spDocument = nullptr;
		
		PoDoFo::PdfFont*					m_pTextFont = nullptr;
		double								m_dIndentLength = 0.0;
		double								m_dLineHeight = 0.0;

		PoDoFo::PdfOutlineItem*				m_pCurrentParentOutlineItem = nullptr;
		QList<TocItem>						m_TocItems;

		std::unique_ptr<PoDoFo::PdfImage>	m_spImgPersons = nullptr;
		std::unique_ptr<PoDoFo::PdfImage>	m_spImgDuration = nullptr;
	};
}

#endif
