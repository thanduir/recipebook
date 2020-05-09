#ifndef RECIPEBOOK_LISTMODEL_SHOPPINGLISTITEMS_H
#define RECIPEBOOK_LISTMODEL_SHOPPINGLISTITEMS_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class ShoppingRecipe;
	class RBDataHandler;
	class RBDataReadHandle;
	class RBDataWriteHandle;
}

namespace recipebook::UI
{
	class ListModelShoppingListItems : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class ShoppingListItemsRoles : int
		{
			NameRole = Qt::UserRole + 1,
			AmountUnitRole,
			AmountIsRangeRole,
			AmountMinRole,
			AmountMaxRole,
			AdditionalInfoRole,
			SizeRole,
			OptionalRole,
			HasGroupRole,
			GroupRole,
			GroupColorRole,
			StatusRole
		};

	public:
		ListModelShoppingListItems(RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	public slots:
		quint32 indexUnitUnitless() const;
        
		void setShoppingRecipe(int row);

		QString name(int row) const;

		bool firstInGroup(int row) const;
		bool lastInGroup(int row) const;
		bool hasGroup(int row) const;

		quint32 amountUnit(int row) const;
		bool amountIsRange(int row) const;
		float amountMin(int row) const;
		float amountMax(int row) const;
		QString additionalInfo(int row) const;
		quint32 sizeIndex(int row) const;
		bool optional(int row) const;
		QString group(int row) const;
		QString groupColor(int row) const;
		quint32 status(int row) const;
        
		void setAmountUnit(int row, quint32 uiUnit);
		void setAmountIsRange(int row, bool bRange);
		void setAmountMin(int row, float amount);
		void setAmountMax(int row, float amount);
		void setAdditionalInfo(int row, QString text);
		void setSizeIndex(int row, quint32 index);
		void setOptional(int row, bool bOptional);
		void setGroup(int row, QString group);
		void setStatus(int row, quint32 uiStatus);

		int addItem(QString strIngredient);
		bool removeItem(int row);
		
		// edit list interface
		void beginEditList();
		bool itemSelected(QString itemName) const;
		void changeState(QString itemName, bool selected);
		void cancelEditList();
		bool applyEditList();

		void onDependentItemChanged(quint32 index);
		void onDataReset() { setShoppingRecipe(-1); }

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, ShoppingListItemsRoles role);

		ShoppingRecipe* getShoppingRecipe(recipebook::RBDataWriteHandle& rHandle);
		const ShoppingRecipe* getShoppingRecipe(recipebook::RBDataReadHandle& rHandle) const;

	private:
		RBDataHandler& m_rRBDataHandler;
		const UIStringConverter& m_rConverter;
		int m_ShoppingRecipeIndex;

		QStringList m_EditListSelectedValues;
		QStringList m_EditListDeselectedValues;
	};
}

#endif
