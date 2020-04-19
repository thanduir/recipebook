#ifndef RECIPEBOOK_DATA_HANDLER_H
#define RECIPEBOOK_DATA_HANDLER_H

#include <QtGlobal>
#include <data/RecipeBook.h>
#include "ListModelCategories.h"
#include "ListModelIngredients.h"
#include "ListModelSortOrders.h"
#include "ListModelSortOrder.h"
#include "ListModelProvenance.h"
#include "ListModelAlternativesGroups.h"
#include "ListModelRecipes.h"
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
        ListModelSortOrder& getSortOrderModel();
        ListModelProvenance& getProvenanceModel();
        ListModelSortOrders& getSortOrdersModel();
        ListModelIngredients& getIngredientsModel();
        ListModelAlternativesGroups& getAlternativesGroups();
        ListModelRecipes& getRecipesModel();
        ListModelRecipeItems& getRecipeItemsModel();

    public slots:
        void slotSaveAs(QString strFileURL);

    private:
        RecipeBook m_RecipeBook;
        UIStringConverter m_Converter;

        ListModelCategories m_ModelCategories;
        ListModelSortOrder m_ModelSortOrder;

        ListModelProvenance m_ModelProvenance;
        ListModelSortOrders m_ModelSortOrders;

        ListModelIngredients m_ModelIngredients;

        ListModelAlternativesGroups m_AlternativesGroups;

        ListModelRecipes m_ModelRecipes;

        ListModelRecipeItems m_ModelRecipeItems;
    };
}

#endif
