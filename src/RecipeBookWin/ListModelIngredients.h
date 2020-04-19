#ifndef RECIPEBOOK_LISTMODEL_INGREDIENTS_H
#define RECIPEBOOK_LISTMODEL_INGREDIENTS_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
    class RecipeBook;
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
            ProvenanceRole,
            DefaultUnitRole
        };

    public:
        ListModelIngredients(RecipeBook& rRecipeBook, const UIStringConverter& rConverter);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    public slots:
        QString name(int row) const;
        QString category(int row) const;
        QString provenance(int row) const;
        QString defaultUnit(int row) const;

        QString listUsedInRecipes(int row) const;
        QString listUsedInShoppingRecipes(int row) const;
                
        void setCategory(int row, QString newCategory);
        void setProvenance(int row, QString newProvenance);
        void setDefaultUnit(int row, QString newDefaultUnit);

        int renameIngredient(int row, QString newName);

        int addIngredient(QString strIngredient);
        bool existsIngredient(QString strIngredient) const;
        int indexOfIngredient(QString strIngredient) const;

        bool canIngredientBeRemoved(int row) const;
        bool removeIngredient(int row);

        void onCategoryRenamed(quint32 index);
        void onSortOrderRenamed(quint32 index);

    signals:
        void ingredientRenamed(quint32 index);

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        void setDataChanged(int row, IngredientRoles role);

    private:
        RecipeBook& m_rRecipeBook;
        const UIStringConverter& m_rConverter;
    };
}

#endif
