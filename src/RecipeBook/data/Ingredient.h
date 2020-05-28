#ifndef RECIPEBOOK_INGREDIENT_H
#define RECIPEBOOK_INGREDIENT_H

#include <QString>
#include "util/RBElementId.h"
#include "Unit.h"

namespace recipebook
{
	class Category;
	class SortOrder;

	class Ingredient
	{
	public:
		QString getName() const { return m_Name; }
		RBElementId getElementId() const { return getElementId(getName()); }

		const Category& getCategory() const { return *m_pCategory; }
		void setCategory(const Category& rCategory) { m_pCategory = &rCategory; }

		bool provenanceAvailable(const SortOrder& rProvenance) const;
		void setProvenanceAvailable(const SortOrder& rProvenance);
		void setProvenanceUnavailable(const SortOrder& rProvenance);

		Unit getDefaultUnit() const { return m_DefaultUnit; }
		void setDefaultUnit(Unit unit) { m_DefaultUnit = unit; }

	private:
		Ingredient(QString strName, const Category& rCategory, Unit defaultUnit);

		Ingredient(const Ingredient& rOther) = delete;
		void operator=(const Ingredient& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

		static RBElementId getElementId(QString strName) { return RBElementId(strName); }

	private:
		QString m_Name;
		const Category* m_pCategory;
		QVector<const SortOrder*> m_UnavailableProvenances;
		Unit m_DefaultUnit;

		friend class RecipeBook;
		friend class RecipeItem;
		friend class ShoppingListItem;
	};
}

#endif
