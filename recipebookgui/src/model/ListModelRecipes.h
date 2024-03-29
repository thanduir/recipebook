#ifndef RECIPEBOOK_LISTMODEL_RECIPES_H
#define RECIPEBOOK_LISTMODEL_RECIPES_H

#include <QAbstractListModel>

namespace recipebook
{
	class RecipeBook;
	class RecipeBookSettings;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelRecipes : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class RecipeRoles : int
		{
			NameRole = Qt::UserRole + 1,
			NumberOfPersonsRole,
			ShortDescriptionRole,
			RecipeTextRole,
			CookingTimeInMinRole,
			EverythingSetRole
		};

	public:
		ListModelRecipes(RBDataHandler& rRBDataHandler, const RecipeBookSettings& rSettings);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		quint32 numberOfPersons(int row) const;
		QString shortDescription(int row) const;
		QString recipeText(int row) const;
		quint32 cookingTime(int row) const;

		bool isEverythingSet(int row) const;

		void setNumberOfPersons(int row, qint32 nrPersons);
		void setShortDescription(int row, QString desc);
		void setRecipeText(int row, QString text);
		void setCookingTime(int row, quint32 timeInMin);
        
		int renameRecipe(int row, QString newName);

		bool canRecipesBeAdded() const;
		int addRecipe(QString strRecipe);
		int copyRecipe(int row, QString strRecipe);
		bool existsRecipe(QString strRecipe) const;

		bool removeRecipe(int row);

		void onDataReset();

	signals:
		void recipeRemoved(int index);

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, RecipeRoles role);

	private:
		RBDataHandler& m_rRBDataHandler;
		const RecipeBookSettings& m_rSettings;
	};
}

#endif
