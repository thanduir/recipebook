#ifndef RECIPEBOOK_SHOPPINGLIST_EXPORTER_LATEX_H
#define RECIPEBOOK_SHOPPINGLIST_EXPORTER_LATEX_H

#include <QObject>

class UIStringConverter;

namespace recipebook
{
	class GoShoppingListItem;
	class SortedShoppingList;
	class RBDialogInterface;

	class ShoppingListExporterLatex : public QObject
	{
		Q_OBJECT

	public:
		ShoppingListExporterLatex(const UIStringConverter& rConverter);

		void exportShoppingList(QString strFilename, const SortedShoppingList& rList, const RBDialogInterface& rDlgInterface);

	private:
		QString generateLatex(const SortedShoppingList& list);

		QString formatItem(const GoShoppingListItem& rItem) const;
		QString getItemAdditionalText(const GoShoppingListItem& rItem) const;

	private:
		const UIStringConverter& m_rConverter;
	};
}

#endif
