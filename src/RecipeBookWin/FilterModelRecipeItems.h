#ifndef RECIPEBOOK_FILTERMODEL_RECIPEITEMS_H
#define RECIPEBOOK_FILTERMODEL_RECIPEITEMS_H

#include <QSortFilterProxyModel>

namespace recipebook::UI
{
	class FilterModelRecipeItems : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		FilterModelRecipeItems();

	public slots:
		void beginMove(int row);
		void updateMoveTarget(int target);
		void applyMove();

	protected:
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

	private:
		int m_MoveFrom = -1;
		int m_MoveTo = -1;
	};
}

#endif
