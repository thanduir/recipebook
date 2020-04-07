#ifndef RECIPEBOOK_DATA_HANDLER_H
#define RECIPEBOOK_DATA_HANDLER_H

#include <QtGlobal>
#include <data/RecipeBook.h>
#include "ListModelCategories.h"
#include "ListModelIngredients.h"
#include "ListModelSortOrders.h"
#include "ListModelSortOrder.h"
#include "ListModelProvenance.h"
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
        QStringList getAllSizeNames() const;
        QStringList getAllStatusNames() const;

        // DataModels
        ListModelCategories& getCategoriesModel();
        ListModelSortOrders& getSortOrdersModel();
        ListModelSortOrder& getSortOrderModel();
        ListModelProvenance& getProvenanceModel();
        ListModelIngredients& getIngredientsModel();

    public slots:
        void slotSaveAs(QString strFileURL);

    private:
        RecipeBook m_RecipeBook;
        UIStringConverter m_Converter;

        ListModelIngredients m_ModelIngredients;
        ListModelCategories m_ModelCategories;
        ListModelSortOrder m_ModelSortOrder;
        ListModelProvenance m_ModelProvenance;
        ListModelSortOrders m_ModelSortOrders;
    };
}

#endif
