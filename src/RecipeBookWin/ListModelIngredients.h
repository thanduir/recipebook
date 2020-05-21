#ifndef RECIPEBOOK_LISTMODEL_INGREDIENTS_H
#define RECIPEBOOK_LISTMODEL_INGREDIENTS_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class RecipeBookSettings;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelIngredients : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class IngredientRoles : int
		{
			NameRole = Qt::UserRole + 1,
			CategoryRole,
			DefaultUnitRole
		};

	public:
		ListModelIngredients(RBDataHandler& rRBDataHandler, const RecipeBookSettings& rSettings, const UIStringConverter& rConverter);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		QString category(int row) const;
		QString defaultUnit(int row) const;
		bool provenanceAvailable(int row, QString strProvenance) const;

		QStringList listUsedInRecipes(int row) const;
		QStringList listUsedInShoppingRecipes(int row) const;
                
		void setCategory(int row, QString newCategory);
		void setDefaultUnit(int row, QString newDefaultUnit);
		void setProvenanceAvailable(int row, QString strProvenance, bool bAvailable);

		int renameIngredient(int row, QString newName);

		bool canIngredientsBeAdded() const;
		int addIngredient(QString strIngredient);
		bool existsIngredient(QString strIngredient) const;
		int indexOfIngredient(QString strIngredient) const;

		bool canIngredientBeRemoved(int row) const;
		bool removeIngredient(int row);

		void onCategoryRenamed(quint32 index);
		void onSortOrderRenamed(quint32 index);

		void onDataReset();

	signals:
		void ingredientRenamed(quint32 index);

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, IngredientRoles role);

	private:
		RBDataHandler& m_rRBDataHandler;
		const RecipeBookSettings& m_rSettings;
		const UIStringConverter& m_rConverter;
	};
}

#endif
