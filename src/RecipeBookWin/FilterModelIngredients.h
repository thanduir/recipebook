#ifndef RECIPEBOOK_FILTERMODEL_RECIPEITEMS_H
#define RECIPEBOOK_FILTERMODEL_RECIPEITEMS_H

#include <QSortFilterProxyModel>

namespace recipebook::UI
{
	class FilterModelIngredients : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		FilterModelIngredients();

	public slots:
		void setFilterString(QString strFilter);    

		QString name(int row) const;        
		QString category(int row) const;
		QString provenance(int row) const;
		QString defaultUnit(int row) const;

		QString listUsedInRecipes(int row) const;
		QString listUsedInShoppingRecipes(int row) const;
                
		void setCategory(int row, QString newCategory);
		void setProvenance(int row, QString newProvenance);
		void setDefaultUnit(int row, QString newDefaultUnit);

		int renameIngredient(int row, QString newName);

		int addIngredient(QString strIngredient);
		bool existsIngredient(QString strIngredient) const;

		bool canIngredientBeRemoved(int row) const;
		bool removeIngredient(int row);

	protected:
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

	private:
		QString m_FilterString;
		QStringList m_AdditionalItemsToShow;
	};
}

#endif
