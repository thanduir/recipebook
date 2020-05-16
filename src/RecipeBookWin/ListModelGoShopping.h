#ifndef RECIPEBOOK_LISTMODEL_GOSHOPPING_H
#define RECIPEBOOK_LISTMODEL_GOSHOPPING_H

#include <QAbstractListModel>
#include <data/SortedShoppingList.h>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class RecipeBookSettings;
	class RBDataHandler;

	enum class GoShoppingListItemType;
}

namespace recipebook::UI
{
	class ListModelGoShopping : public QAbstractListModel, 
								public recipebook::SortedShoppingList::ListChangeCallback
	{
		Q_OBJECT
	public:
		enum class GoShoppingRoles : int
		{
			NameRole = Qt::UserRole + 1,
			TopLevelHeaderRole,
			CategoryHeaderRole,
			IncompatibleItemsHeaderRole,
			NormalItemRole,
			ItemCheckedRole
		};

	public:
		ListModelGoShopping(RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	public slots:
		void updateList();
		void setSortOrder(QString strSortOrder, bool bSeparateCollectedItems);

		QString name(int row) const;
		bool isItemChecked(int row) const;
		
		bool isTopLevelHeader(int row) const;
		bool isCategoryHeader(int row) const;
		bool isIncompatibleItemsHeader(int row) const;
		bool isNormalItem(int row) const;

		void setItemChecked(int row, bool bChecked);

		void onDataReset();

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		GoShoppingListItemType itemType(int row) const;
		void setDataChanged(int row, GoShoppingRoles role);

		// Methods from SortedShoppingList::ListChangeCallback
		virtual void itemChanged(int posItem) override;
		virtual void beginMoveItem(int currentItemPos, int newItemPos) override;
		virtual void endMoveItem() override;
		virtual void beginInsertItem(int posItem) override;
		virtual void endInsertItem() override;
		virtual void beginRemoveItem(int posItem) override;
		virtual void endRemoveItem() override;

	private:
		RBDataHandler&				m_rRBDataHandler;
		const UIStringConverter&	m_rConverter;

		SortedShoppingList			m_List;

		friend class GoShoppingCallback;
	};
}

#endif
