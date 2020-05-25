#ifndef RECIPEBOOK_SHOPPINGLIST_EXPORTER_QTHTML_H
#define RECIPEBOOK_SHOPPINGLIST_EXPORTER_QTHTML_H

#include <QObject>

class UIStringConverter;

namespace recipebook
{
	class GoShoppingListItem;
	class SortedShoppingList;

	class ShoppingListExporterQtHtml : public QObject
	{
		Q_OBJECT

	public:
		ShoppingListExporterQtHtml(const UIStringConverter& rConverter);

		void exportShoppingList(QString strFilename, const SortedShoppingList& rList);

	private:
		void exportPdf(QString strFilename, QString strFormattedText);
		QString generateHTML(const SortedShoppingList& list);
		
		QString formatItem(const GoShoppingListItem& rItem) const;
		QString getItemAdditionalText(const GoShoppingListItem& rItem) const;

	private:
		const UIStringConverter& m_rConverter;
	};
}

#endif
