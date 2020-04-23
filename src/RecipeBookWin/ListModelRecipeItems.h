#ifndef RECIPEBOOK_LISTMODEL_RECIPEITEMS_H
#define RECIPEBOOK_LISTMODEL_RECIPEITEMS_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
    class RecipeBook;
    class Recipe;
    class RecipeItemGroup;
}

namespace recipebook::UI
{
    class ListModelRecipeItems : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum class RecipeItemsRoles : int
        {
            NameRole = Qt::UserRole + 1,
            AmountUnitRole,
            AmountIsRangeRole,
            AmountMinRole,
            AmountMaxRole,
            AdditionalInfoRole,
            SizeRole,
            OptionalRole,
            GroupRole,
            GroupColorRole
        };

    public:
        ListModelRecipeItems(RecipeBook& rRecipeBook, const UIStringConverter& rConverter);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    public slots:
        quint32 indexUnitUnitless() const;
        
        void setRecipe(int row);

        QString name(int row) const;

        quint32 amountUnit(int row) const;
        bool amountIsRange(int row) const;
        float amountMin(int row) const;
        float amountMax(int row) const;
        QString additionalInfo(int row) const;
        quint32 sizeIndex(int row) const;
        bool optional(int row) const;
        QString group(int row) const;
        QString groupColor(int row) const;
        
        void setAmountUnit(int row, quint32 uiUnit);
        void setAmountIsRange(int row, bool bRange);
        void setAmountMin(int row, float amount);
        void setAmountMax(int row, float amount);
        void setAdditionalInfo(int row, QString text);
        void setSizeIndex(int row, quint32 index);
        void setOptional(int row, bool bOptional);
        void setGroup(int row, QString group);

        int addRecipeItem(QString strIngredient);
        bool removeItem(int row);
        void moveItem(int row, int target);

        void onDependentItemChanged(quint32 index);

        // edit list interface
        void beginEditList();
        bool itemSelected(QString itemName);
        void changeState(QString itemName, bool selected);
        void cancelEditList();
        bool applyEditList();

        void onDataReset() { setRecipe(-1); }

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        void setDataChanged(int row, RecipeItemsRoles role);

    private:
        RecipeBook& m_rRecipeBook;
        const UIStringConverter& m_rConverter;
        Recipe* m_pRecipe;

        QStringList m_EditListSelectedValues;
        QStringList m_EditListDeselectedValues;
    };
}

#endif
