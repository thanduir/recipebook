#ifndef RECIPEBOOK_LISTMODEL_SORTORDERS_H
#define RECIPEBOOK_LISTMODEL_SORTORDERS_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelSortOrders : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class SortOrdersRoles : int
		{
			NameRole = Qt::UserRole + 1
		};
    
	public:
		ListModelSortOrders(RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		int renameSortOrder(int row, QString newName);

		bool canSortOrdersBeAdded() const;
		int addSortOrder(QString strSortOrder);
		bool existsSortOrder(QString strSortOrder) const;

		bool removeSortOrder(int row);

		void onDataReset();
        
	signals:
		void sortOrderRenamed(quint32 index);

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		RBDataHandler& m_rRBDataHandler;
		const UIStringConverter& m_rConverter;
	};
}

#endif
