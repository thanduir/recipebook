#ifndef RB_ELEMENT_ID_H
#define RB_ELEMENT_ID_H

#include <QString>

namespace recipebook
{
	class RBElementId;
	namespace helper
	{
		inline int compare(const RBElementId& idFirst, const RBElementId& idSecond);
	}

	class RBElementId
	{
	public:
		QString toString() const { return m_Id; }

	private:
		explicit RBElementId(QString strId) : m_Id(strId) {}

	private:
		QString m_Id;

		friend int helper::compare(const RBElementId& idFirst, const RBElementId& idSecond);

		friend class Category;
		friend class SortOrder;
		friend class Ingredient;
		friend class AlternativesType;
		friend class Recipe;
		friend class RecipeItem;
		friend class ShoppingRecipe;
		friend class ShoppingListItem;
		friend class GoShoppingListItem;
		friend class RecipeBookConfiguration;
		friend class RecipeBookConfigItem;
	};

	namespace helper
	{
		inline int compare(const RBElementId& idFirst, const RBElementId& idSecond)
		{
			return QString::localeAwareCompare(idFirst.m_Id.toLower(), idSecond.m_Id.toLower());
		}

		inline bool lessThan(const RBElementId& idFirst, const RBElementId& idSecond)
		{
			return compare(idFirst, idSecond) < 0;
		}
	}
}

#endif
