#ifndef RECIPEBOOK_CHANGES_H
#define RECIPEBOOK_CHANGES_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

namespace recipebook
{
	class RecipeBook;
	class Recipe;
	class RecipeBookConfiguration;

	class RecipeBookChanges final : public QObject
	{
		Q_OBJECT

	public:
		struct ItemChanges
		{
			QStringList	m_Added;
			QStringList	m_Removed;
			QStringList	m_Changed;
		};

	public:
		RecipeBookChanges();

		void compare(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);

	// TODO: Needed as slots?
	public slots:
		const ItemChanges& getCategoryChanges() const { return m_CategoryChanges; }
		const ItemChanges& getSortOrderChanges() const { return m_SortOrderChanges; }
		const ItemChanges& getAlternativeTypesChanges() const { return m_AlternativesTypesChanges; }
		const ItemChanges& getIngredientsChanges() const { return m_IngredientsChanges; }

		const ItemChanges& getRecipesChanges() const { return m_RecipesChanges; }
		const ItemChanges getRecipeChanges(QString strRecipe) const;

		const ItemChanges& getRBConfigurationsChanges() const { return m_RBConfigurationsChanges; }
		const ItemChanges getRBConfigItemChanges(QString strConfiguration) const;

	private:
		void clear();

		void compareCategories(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);
		void compareSortOrders(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);
		void compareAlternativesTypes(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);
		void compareIngredients(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);
		void compareRecipes(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);
		bool compareRecipeItems(QString strRecipe, 
								const Recipe& rRecipeBase, 
								const Recipe& rRecipeChanged, 
								const QSharedPointer<RecipeBook> spBase,
								const QSharedPointer<RecipeBook> spChanged);

		void compareRBConfigurations(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged);
		bool compareRBConfigItems(QString strConfiguration,
								  const RecipeBookConfiguration& rConfigBase,
								  const RecipeBookConfiguration& rConfigChanged,
								  const QSharedPointer<RecipeBook> spBase,
								  const QSharedPointer<RecipeBook> spChanged);

		void clearChanges(ItemChanges &rChanges)
		{
			rChanges.m_Added.clear();
			rChanges.m_Removed.clear();
			rChanges.m_Changed.clear();
		}

	private:
		ItemChanges					m_CategoryChanges;
		ItemChanges					m_SortOrderChanges;
		ItemChanges					m_AlternativesTypesChanges;
		ItemChanges					m_IngredientsChanges;

		ItemChanges					m_RecipesChanges;
		QMap<QString, ItemChanges>	m_RecipeItemChanges;

		ItemChanges					m_RBConfigurationsChanges;
		QMap<QString, ItemChanges>	m_RBConfigItemChanges;
	};
}

#endif
