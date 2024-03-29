#ifndef RECIPEBOOK_RECIPE_H
#define RECIPEBOOK_RECIPE_H

#include <QString>
#include <QSharedPointer>
#include <QTime>
#include "RBElementId.h"

namespace recipebook
{
	class RecipeItem;
	class Ingredient;

	class Recipe
	{
	public:
		QString getName() const { return m_Name; }
		RBElementId getElementId() const { return getElementId(getName()); }
        
		quint32 getNumberOfPersons() const { return m_NrPersons; }
		void setNumberOfPersons(quint32 number) { m_NrPersons = number; }

		QString getShortDescription() const { return m_ShortDescription; }
		void setShortDescription(QString strText) { m_ShortDescription = strText; }

		QString getRecipeText() const { return m_RecipeText; }
		void setRecipeText(QString strText) { m_RecipeText = strText; }

		QTime getCookingTime() const { return m_CookingTime; }
		void setCookingTime(QTime duration) { m_CookingTime = duration; }

		// Recipe items
        
		RecipeItem& addRecipeItem(const Ingredient& rIngredient);
		bool existsRecipeItem(const Ingredient& rIngredient) const;
		bool removeRecipeItem(const RecipeItem& rItem);
		RecipeItem& getRecipeItem(const Ingredient& rIngredient);
		const RecipeItem& getRecipeItem(const Ingredient& rIngredient) const;
        
		quint32 getRecipeItemsCount() const;
		RecipeItem& getRecipeItemAt(quint32 i);
		const RecipeItem& getRecipeItemAt(quint32 i) const;
		void moveRecipeItem(const RecipeItem& rItem, quint32 newPos);

        int getRecipeItemIndex(QString strName) const;

		// Returns true if this is already the case
		bool moveGroupItemsTogether(); 
		
		bool operator!=(const Recipe& rOther) const { return !(*this == rOther); }
		bool operator==(const Recipe& rOther) const;

	private:
        explicit Recipe(QString strName, quint32 uiNrPersons) : m_Name(strName), m_NrPersons(uiNrPersons), m_CookingTime(0, 30) {}
		Recipe(QString strName, const Recipe& rOther);

		void operator=(const Recipe& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

		static RBElementId getElementId(QString strName) { return RBElementId(strName); }

	private:
		QString m_Name;
		quint32 m_NrPersons;
		QString m_ShortDescription;
		QString m_RecipeText;
		QTime m_CookingTime;

		QVector<QSharedPointer<RecipeItem>> m_RecipeItems;

		friend class RecipeBook;
	};
}

#endif
