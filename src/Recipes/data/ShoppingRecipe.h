#ifndef RECIPEBOOK_SHOPPINGRECIPE_H
#define RECIPEBOOK_SHOPPINGRECIPE_H

#include <QString>

namespace recipebook
{
    class Recipe;

    class ShoppingRecipe
    {
    public:
        QString getName() const { return m_Name; }
        void rename(QString strNewName) { m_Name = strNewName; }

    private:
        explicit ShoppingRecipe(QString strName) { m_Name = strName; }
        explicit ShoppingRecipe(const Recipe& rRecipe);

        ShoppingRecipe(const ShoppingRecipe& rOther) = delete;
        void operator=(const ShoppingRecipe& rOther) = delete;

    private:
        QString m_Name;

        friend class RecipeBook;
    };
}

// TODO!
/*
public static abstract class ShoppingRecipe
{
    public abstract String getName();

    // Current scaling factor used for the items in the list.
    public abstract float getScalingFactor();
    public abstract void setScalingFactor(float factor);

    public abstract Optional<ShoppingListItem> addItem(@NonNull String strIngredient);
    public abstract void addItem(@NonNull RecipeItem recipeItem);
    public abstract void removeItem(@NonNull ShoppingListItem r);
    public abstract ArrayList<ShoppingListItem> getAllItems();

    public abstract void changeScalingFactor(float f);
}
*/
/*public abstract class ShoppingListItem
{
    public enum Status
    {
        None,
        Taken
    }

    public abstract Status getStatus();
    public abstract void setStatus(@NonNull Status status);
    public abstract void invertStatus();

    public abstract String getIngredient();
    public abstract void setIngredient(@NonNull String strIngredient);

    public abstract Amount getAmount();
    public abstract void setAmount(@NonNull Amount amount);

    public abstract String getAdditionalInfo();
    public abstract void setAdditionInfo(@NonNull String additionalInfo);

    public abstract RecipeItem.Size getSize();
    public abstract void setSize(@NonNull RecipeItem.Size size);

    public abstract boolean isOptional();
    public abstract void setIsOptional(boolean optional);
}*/
#endif
