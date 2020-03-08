#ifndef RECIPEBOOK_RECIPE_H
#define RECIPEBOOK_RECIPE_H

#include <QString>

namespace recipebook
{
    class Recipe
    {
    public:
        QString getName() const { return m_Name; }
        void rename(QString strNewName) { m_Name = strNewName; }

    private:
        explicit Recipe(QString strName) { m_Name = strName; }      
        Recipe(const Recipe& rOther);

        void operator=(const Recipe& rOther) = delete;

    private:
        QString m_Name;

        friend class RecipeBook;
    };
}

// TODO!
/*  public static abstract class Recipe implements Helper.NamedObject
    {
        public abstract int getNumberOfPersons();
        public abstract void setNumberOfPersons(int number);

        public abstract Optional<RecipeItem> addRecipeItem(@NonNull String strIngredient);
        public abstract void removeRecipeItem(@NonNull RecipeItem r);
        public abstract ArrayList<RecipeItem> getAllRecipeItems();

        // Groups

        public abstract void addGroup(@NonNull String strName);
        public abstract void removeGroup(@NonNull String strName);
        public abstract void renameGroup(@NonNull String strOldName, @NonNull String strNewName);
        public abstract ArrayList<String> getAllGroupNames();

        public abstract Optional<RecipeItem> addRecipeItemToGroup(@NonNull String strGroup, @NonNull String strIngredient);
        public abstract void removeRecipeItemFromGroup(@NonNull String strGroup, @NonNull RecipeItem r);
        public abstract ArrayList<RecipeItem> getAllRecipeItemsInGroup(@NonNull String strGroup);
    }
}*/

/*public interface RecipeItem
{
    enum Size
    {
        Small,
        Normal,
        Large;

        public static String toUIString(@NonNull Context context, @NonNull Size size)
        {
            switch(size)
            {
                case Normal:
                    return context.getResources().getString(R.string.size_normal);

                case Small:
                    return context.getResources().getString(R.string.size_small);

                case Large:
                    return context.getResources().getString(R.string.size_large);

                default:
                    return size.toString();
            }
        }
    }

    String getIngredient();
    void setIngredient(@NonNull String strIngredient);

    Amount getAmount();
    void setAmount(@NonNull Amount amount);

    String getAdditionalInfo();
    void setAdditionInfo(@NonNull String additionalInfo);

    Size getSize();
    void setSize(@NonNull Size size);

    boolean isOptional();
    void setIsOptional(boolean optional);
}*/

#endif
