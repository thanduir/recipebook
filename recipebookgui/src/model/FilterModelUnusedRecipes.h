#ifndef RECIPEBOOK_FILTERMODEL_UNUSED_RECIPES_H
#define RECIPEBOOK_FILTERMODEL_UNUSED_RECIPES_H

#include <QSortFilterProxyModel>

namespace recipebook
{
	class RBDataHandler;
}

namespace recipebook::UI
{
	class FilterModelUnusedRecipes : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		FilterModelUnusedRecipes(RBDataHandler& rRBDataHandler);

	public slots:
		void setRecipeBookConfiguration(quint32 uiConfig);
		void setFilterString(QString strFilter);

	protected:
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

	private:
		RBDataHandler& m_rRBDataHandler;
		int m_iConfig = -1;
		QString m_FilterString;
	};
}

#endif
