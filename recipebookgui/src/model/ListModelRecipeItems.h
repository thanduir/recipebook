#ifndef RECIPEBOOK_LISTMODEL_RECIPEITEMS_H
#define RECIPEBOOK_LISTMODEL_RECIPEITEMS_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class Recipe;
	class RBDataHandler;
	class RBDataReadHandle;
	class RBDataWriteHandle;
}

namespace recipebook::UI
{
	class ListModelRecipeItems : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class RecipeItemsRoles : int
		{
			NameRole = Qt::UserRole + 1,
			GroupOrItemNameRole,
			AmountUnitRole,
			AmountIsRangeRole,
			AmountMinRole,
			AmountMaxRole,
			AdditionalInfoRole,
			SizeRole,
			OptionalRole,
			HasGroupRole,
			GroupRole,
			GroupColorRole
		};
		QMap<int, bool> m_Held;

	public:
		ListModelRecipeItems(RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	public slots:
		quint32 indexUnitUnitless() const;
        
		void setRecipe(int row);

		QString name(int row) const;
		QString groupOrItemName(int row) const;

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
        
		void setAmountUnit(int row, quint32 uiUnit);
		void setAmountIsRange(int row, bool bRange);
		void setAmountMin(int row, float amount);
		void decreaseAmountMin(int row); 
		void increaseAmountMin(int row);
		void setAmountMax(int row, float amount);
		void decreaseAmountMax(int row); 
		void increaseAmountMax(int row);
		void setAdditionalInfo(int row, QString text);
		void setSizeIndex(int row, quint32 index);
		void setOptional(int row, bool bOptional);
		int setGroup(int row, QString group);

		bool canRecipeItemsBeAdded() const;
		int addRecipeItem(QString strIngredient);
		bool removeItem(int row);
		void moveItem(int row, int target);

		void onDependentItemChanged(quint32 index);

		// edit list interface
		void beginEditList();
		bool itemSelected(QString itemName) const;
		void changeState(QString itemName, bool selected);
		void cancelEditList();
		bool applyEditList();

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, RecipeItemsRoles role);

		Recipe* getRecipe(recipebook::RBDataWriteHandle& rHandle);
		const Recipe* getRecipe(recipebook::RBDataReadHandle& rHandle) const;

	private:
		RBDataHandler& m_rRBDataHandler;
		const UIStringConverter& m_rConverter;
		int m_RecipeIndex;

		QStringList m_EditListSelectedValues;
		QStringList m_EditListDeselectedValues;
	};
}

#endif
