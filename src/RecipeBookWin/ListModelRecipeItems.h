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
            OptionalRole
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

        bool isRegularItem(int row) const;
        bool isGroupHeader(int row) const;
        bool isGroupItem(int row) const;

        quint32 amountUnit(int row) const;
        bool amountIsRange(int row) const;
        float amountMin(int row) const;
        float amountMax(int row) const;
        QString additionalInfo(int row) const;
        quint32 sizeIndex(int row) const;
        bool optional(int row) const;
        
        void setAmountUnit(int row, quint32 uiUnit);
        void setAmountIsRange(int row, bool bRange);
        void setAmountMin(int row, float amount);
        void setAmountMax(int row, float amount);
        void setAdditionalInfo(int row, QString text);
        void setSizeIndex(int row, quint32 index);
        void setOptional(int row, bool bOptional);

        int addRecipeGroup(QString strName);
        // TODO: String or index for ingredient? index might make more sense, depending on how this is done in UI
        //int addRecipeItem(QString strName, int groupIndex);
        
        bool existsGroup(QString strGroup) const;

        int renameGroup(int row, QString newName);

        bool removeItem(int row);

        void onIngredientRenamed(quint32 index);

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        enum class ItemType
        {
            None,
            RegularItem,
            GroupHeader,
            GroupItem
        };

        struct ItemInfo
        {
            ItemType type           = ItemType::None;
            quint32 itemIndex       = 0;
            RecipeItemGroup* pGroup = nullptr;
        };

    private:
        void setDataChanged(int row, RecipeItemsRoles role);
        void getItemInfo(int row, ItemInfo& rInfo) const;

    private:
        RecipeBook& m_rRecipeBook;
        const UIStringConverter& m_rConverter;
        Recipe* m_pRecipe;
    };
}

#endif
