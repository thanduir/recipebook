#ifndef RECIPEBOOK_LISTMODEL_CATEGORIES_H
#define RECIPEBOOK_LISTMODEL_CATEGORIES_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
    class RecipeBook;
}

namespace recipebook::UI
{
    // TODO(phiwid): Do I actually need this?
    class ListModelCategories : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum class CategoryRoles : int
        {
            NameRole = Qt::UserRole + 1
        };

    public:
        ListModelCategories(RecipeBook& rRecipeBook);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        // TODO: FOR OVERRIDE: also implement setData() and flags(), cf. qabstractdatalistmodel doc!
        // TODO: possibly also implement insertRows() and removeRows() or is this not suitable for my purposes?

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        RecipeBook& m_rRecipeBook;
    };
}

#endif
