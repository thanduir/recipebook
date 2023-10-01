#if 0

namespace recipebook
{
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

#pragma once

#include <memory>
#include <QObject>

class UIStringConverter;

namespace recipebook
{
	class SortedShoppingList;

	bool exportShoppingListLibHaru(const UIStringConverter& converter, const SortedShoppingList& config, QString filename);
}
