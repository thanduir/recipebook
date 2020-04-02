#ifndef RECIPEBOOK_LISTMODEL_PROVENANCE_H
#define RECIPEBOOK_LISTMODEL_PROVENANCE_H

#include "ListModelSortOrders.h"

class UIStringConverter;

namespace recipebook::UI
{
    class ListModelProvenance : public ListModelSortOrders
    {
        Q_OBJECT
    
    public:
        ListModelProvenance(RecipeBook& rRecipeBook, const UIStringConverter& rConverter);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    public slots:
        QString provenanceEverywhere() const;

    private:
        const UIStringConverter& m_rConverter;
    };
}

#endif
