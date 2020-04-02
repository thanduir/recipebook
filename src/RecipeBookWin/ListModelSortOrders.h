#ifndef RECIPEBOOK_LISTMODEL_SORTORDER_H
#define RECIPEBOOK_LISTMODEL_SORTORDER_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
    class RecipeBook;
}

namespace recipebook::UI
{
    class ListModelSortOrders : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum class SortOrderRoles : int
        {
            NameRole = Qt::UserRole + 1,
            /*CategoryRole,
            ProvenanceRole,
            DefaultUnitRole*/
        };

    public:
        ListModelSortOrders(RecipeBook& rRecipeBook);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        // TODO: FOR OVERRIDE: also implement setData() and flags(), cf. qabstractdatalistmodel doc!
        // TODO: possibly also implement insertRows() and removeRows() or is this not suitable for my purposes?

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    protected:
        RecipeBook& m_rRecipeBook;
    };
}

#endif
