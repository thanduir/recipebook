#ifndef RECIPEBOOK_DATA_HANDLER_H
#define RECIPEBOOK_DATA_HANDLER_H

#include <QtGlobal>
#include <data/RecipeBook.h>
#include "ListModelCategories.h"
#include "ListModelIngredients.h"
#include "FilterModelIngredients.h"
#include "FilterModelSortOrders.h"
#include "SortModelSortOrder.h"
#include "ListModelProvenance.h"
#include "ListModelAlternativesGroups.h"
#include "FilterModelAlternativesTypes.h"
#include "ListModelRecipes.h"
#include "FilterModelRecipes.h"
#include "ListModelRecipeItems.h"
#include "uistringconverter.h"

namespace recipebook::UI
{
    class RecipeBookDataHandler : public QObject
    {
        Q_OBJECT

    public:
        RecipeBookDataHandler();

        // Namelists
        QStringList getAllUnitNames() const;
        QStringList getAllUnitShortNames() const;
        QStringList getAllSizeNames() const;
        QStringList getAllStatusNames() const;

        // DataModels
        ListModelCategories& getCategoriesModel();
        SortModelSortOrder& getSortOrderModel();
        ListModelProvenance& getProvenanceModel();
        FilterModelSortOrders& getSortOrdersModel();
        ListModelIngredients& getIngredientsModel();
        FilterModelIngredients& getIngredientsFilterModel();
        ListModelAlternativesGroups& getAlternativesGroups();
        FilterModelAlternativesTypes& getAlternativesTypes();
        ListModelRecipes& getRecipesModel();
        FilterModelRecipes& getRecipesFilterModel();
        ListModelRecipeItems& getRecipeItemsModel();

    public slots:
        void slotSaveAs(QString strFileURL);

    private:
        RecipeBook m_RecipeBook;
        UIStringConverter m_Converter;

        ListModelCategories m_ModelCategories;
        SortModelSortOrder m_ModelSortOrder;

        ListModelProvenance m_ModelProvenance;
        FilterModelSortOrders m_ModelSortOrders;

        ListModelIngredients m_ModelIngredients;
        FilterModelIngredients m_FilterModelIngredients;

        ListModelAlternativesGroups m_AlternativesGroups;
        FilterModelAlternativesTypes m_AlternativesTypes;

        ListModelRecipes m_ModelRecipes;
        FilterModelRecipes m_FilterModelRecipes;

        ListModelRecipeItems m_ModelRecipeItems;
    };
}

#endif
