#ifndef RECIPEBOOK_LISTMODEL_SHOPPINGRECIPES_H
#define RECIPEBOOK_LISTMODEL_SHOPPINGRECIPES_H

#include <QAbstractListModel>
#include <QDate>

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
			DueDateRole
		};

	public:
		ListModelShoppingRecipes(RBDataHandler& rRBDataHandler, const RecipeBookSettings& rSettings);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		float scalingFactor(int row) const;
		QDate dueDate(int row) const;

		void setScalingFactor(int row, float fFactor);
		void setDueDate(int row, QDate date);
        
		int renameRecipe(int row, QString newName);

		int addFromRecipe(QString strRecipe);
		int addNewRecipe(QString strName);
		bool existsRecipe(QString strRecipe) const;

		bool removeRecipe(int row);

		void resetShoppingList();

		void onDataReset();

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, ShoppingRecipeRoles role);

	private:
		RBDataHandler& m_rRBDataHandler;
		const RecipeBookSettings& m_rSettings;
	};
}

#endif
