#ifndef RECIPEBOOK_FILTERMODEL_SORTORDERS_H
#define RECIPEBOOK_FILTERMODEL_SORTORDERS_H

#include <QSortFilterProxyModel>

namespace recipebook::UI
{
	class FilterModelSortOrders : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		FilterModelSortOrders();

	public slots:
		QString name(int row) const;
		int renameSortOrder(int row, QString newName);

		int addSortOrder(QString strSortOrder);
		bool existsSortOrder(QString strSortOrder) const;

		QString listUsedInIngredients(int row) const;
		bool canSortOrderBeRemoved(int row) const;
		bool removeSortOrder(int row);

	protected:
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
	};
}

#endif
