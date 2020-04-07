#ifndef RECIPEBOOK_LISTMODEL_SORTORDER_H
#define RECIPEBOOK_LISTMODEL_SORTORDER_H

#include <QSortFilterProxyModel>

class UIStringConverter;

namespace recipebook
{
    class RecipeBook;
    class SortOrder;
}

namespace recipebook::UI
{

    class ListModelSortOrder : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        ListModelSortOrder(RecipeBook& rRecipeBook);

    public slots:
        void setSortOrder(int row);

        QString name(int row) const;
        int renameCategory(int row, QString newName);

        int addCategory(QString strCategory);
        bool existsCategory(QString strCategory) const;

        QString listUsedInIngredients(int row) const;
        bool canCategoryBeRemoved(int row) const;
        bool removeCategory(int row);

    protected:
        virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

    private:
        RecipeBook& m_rRecipeBook;
        SortOrder* m_pSortOrder;
    };
}

#endif
