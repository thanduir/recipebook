#ifndef RECIPEBOOK_LISTMODEL_SORTORDER_H
#define RECIPEBOOK_LISTMODEL_SORTORDER_H

#include <QSortFilterProxyModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class SortModelSortOrder : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		SortModelSortOrder(RBDataHandler& rRBDataHandler);

	public slots:
		void setSortOrder(int row);

		QString name(int row) const;
		int renameCategory(int row, QString newName);

		int addCategory(QString strCategory);
		bool existsCategory(QString strCategory) const;

		QString listUsedInIngredients(int row) const;
		bool canCategoryBeRemoved(int row) const;
		bool removeCategory(int row);

		void beginMoveCategory(int row);
		void updateMoveTarget(int target);
		void applyMoveCategory();

		void onDataReset() { setSortOrder(-1); }

	protected:
		virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

	private:
		RBDataHandler& m_rRBDataHandler;
		int m_SortOrderIndex;

		int m_MoveFrom = -1;
		int m_MoveTo = -1;
	};
}

#endif
