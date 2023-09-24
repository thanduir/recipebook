#ifndef RECIPEBOOK_SHOPPINGLIST_EXPORTER_PODOFO_H
#define RECIPEBOOK_SHOPPINGLIST_EXPORTER_PODOFO_H

#include <memory>
#include <QObject>

#include "podofo.h"

class UIStringConverter;

namespace PoDoFo
{
	class PdfMemDocument;
	class PdfPage;
	class PdfFont;
	class PdfPainter;
	class PdfString;
}

namespace recipebook
{
	class SortedShoppingList;
	class GoShoppingListItem;
	
	class ShoppingListExporterPodofo : public QObject
	{
		Q_OBJECT

	public:
		explicit ShoppingListExporterPodofo(const UIStringConverter& rConverter);
		~ShoppingListExporterPodofo();

		bool writeDocument(QString strFilename, const SortedShoppingList& rList);

	private:
		void addItemAdditionalText(const GoShoppingListItem& rItem, 
								   PoDoFo::PdfPage** pPage,
								   PoDoFo::PdfPainter& rPainter, 
								   double startX, 
								   double& currentX,
								   double& currentY, 
								   double titleLength);
		QString formatItem(const GoShoppingListItem& rItem) const;

		void addPageIfNeeded(PoDoFo::PdfPage** pPage,
							 PoDoFo::PdfPainter& rPainter, 
							 double& currentX,
							 double& rCurrentY);
		
		PoDoFo::PdfFont* createFont(bool bBold, bool bItalic = false);
		PoDoFo::PdfString convertString(QString strString);

	private:
		const UIStringConverter& m_rConverter;

		std::unique_ptr<PoDoFo::PdfMemDocument> m_spDocument = nullptr;
		
		PoDoFo::PdfFont*					m_pTextFont = nullptr;
		double								m_dIndentLength = 0.0;
		double								m_dLineHeight = 0.0;
	};
}

#endif
