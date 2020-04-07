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

    public slots:
        QString name(int row) const;
        int renameCategory(int row, QString newName);

        int addCategory(QString strCategory);
        bool existsCategory(QString strCategory) const;

        QString listUsedInIngredients(int row) const;
        bool canCategoryBeRemoved(int row) const;
        bool removeCategory(int row);

    signals:
        void categoryRenamed(quint32 index);

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        RecipeBook& m_rRecipeBook;
    };
}

#endif
