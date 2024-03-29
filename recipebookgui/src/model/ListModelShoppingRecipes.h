#ifndef RECIPEBOOK_LISTMODEL_SHOPPINGRECIPES_H
#define RECIPEBOOK_LISTMODEL_SHOPPINGRECIPES_H

#include <QAbstractListModel>
#include <QDate>
#include "ListModelShoppingListItems.h"

namespace recipebook
{
	class RecipeBook;
	class RecipeBookSettings;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelShoppingRecipes : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class ShoppingRecipeRoles : int
		{
			NameRole = Qt::UserRole + 1,
			ScalingFactorRole,
			DueDateRole,
			DueDateSetRole,
			EverythingSetRole
		};

	public:
		ListModelShoppingRecipes(RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter, const RecipeBookSettings& rSettings);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		float scalingFactor(int row) const;
		QDate dueDate(int row) const;
		bool isDueDateSet(int row) const;

		bool everythingSet(int row) const;

		void setScalingFactor(int row, float fFactor);
		void setDueDate(int row, QDate date);
		void resetDueDate(int row);
        
		int renameRecipe(int row, QString newName);

		bool canShoppingRecipesBeAddedFromRecipes() const;
		bool canNewShoppingRecipesBeAdded() const;
		int addFromRecipe(QString strRecipe);
		int addNewRecipe(QString strName);
		bool existsRecipe(QString strRecipe) const;

		bool removeRecipe(int row);

		void resetShoppingList();

		void onDataReset();

		void beginAddList();
		quint32 getInsertionsCount(QString itemName) const;
		void changeState(QString itemName, quint32 insertionsCount);
		void cancelAddList();
		bool applyAddList();

		void onItemEnabledChanged(int row);

		ListModelShoppingListItems* getItemsModel(int row);

	signals:
		void recipeScalingChanged(quint32 index);

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, ShoppingRecipeRoles role);

	private:
		RBDataHandler& m_rRBDataHandler;
		const UIStringConverter& m_rConverter;
		const RecipeBookSettings& m_rSettings;

		QMap<QString, quint32> m_AddListValues;
	};
}

#endif
