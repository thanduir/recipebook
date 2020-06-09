#ifndef RECIPEBOOK_SHOPPINGLIST_EXPORTER_H
#define RECIPEBOOK_SHOPPINGLIST_EXPORTER_H

#include <QObject>

class UIStringConverter;

namespace recipebook
{
	class RBDataHandler;
	class RecipeBookSettings;
	class RBDialogInterface;

	class ShoppingListExporter : public QObject
	{
		Q_OBJECT

	public:
		ShoppingListExporter(RBDataHandler& rRBDataHandler, 
							 RecipeBookSettings& rSettings, 
							 const UIStringConverter& rConverter,
							 const RBDialogInterface& rDlgInterface);

	public slots:
		bool exportAvailable() const;

		QStringList getDlgNameFilters() const;
		void exportShoppingList(QString strFileURL, QString strSortOrder);

	private:
		RBDataHandler&				m_rRBDataHandler;
		RecipeBookSettings&			m_rSettings;
		const UIStringConverter&	m_rConverter;
		const RBDialogInterface&	m_rDlgInterface;
	};
}

#endif
