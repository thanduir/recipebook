#ifndef RECIPEBOOK_FILTERMODEL_RECIPES_H
#define RECIPEBOOK_FILTERMODEL_RECIPES_H

#include <QSortFilterProxyModel>

namespace recipebook::UI
{
	class FilterModelRecipes : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		FilterModelRecipes();

	public slots:
		void setFilterString(QString strFilter);    

		QString name(int row) const;
		quint32 numberOfPersons(int row) const;
		QString shortDescription(int row) const;
		QString recipeText(int row) const;
		quint32 cookingTime(int row) const;

		bool isEverythingSet(int row) const;

		void setNumberOfPersons(int row, qint32 nrPersons);
		void setShortDescription(int row, QString desc);
		void setRecipeText(int row, QString text);
		void setCookingTime(int row, quint32 timeInMin);
        
		int renameRecipe(int row, QString newName);

		int addRecipe(QString strRecipe);
		int copyRecipe(int row, QString strRecipe);
		bool existsRecipe(QString strRecipe) const;

		bool removeRecipe(int row);

	protected:
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

	private:
		QString m_FilterString;
		QStringList m_AdditionalItemsToShow;
	};
}

#endif
