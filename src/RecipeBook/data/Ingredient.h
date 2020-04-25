#ifndef RECIPEBOOK_INGREDIENT_H
#define RECIPEBOOK_INGREDIENT_H

#include <QString>
#include "Unit.h"

namespace recipebook
{
	class Category;
	class SortOrder;

	class Ingredient
	{
	public:
		QString getName() const { return m_Name; }

		const Category& getCategory() const { return *m_pCategory; }
		void setCategory(const Category& rCategory) { m_pCategory = &rCategory; }

		bool hasProvenanceEverywhere() const { return m_pProvenance == nullptr; }
		const SortOrder& getProvenance() const;
		void setProvenanceEverywhere();
		void setProvenance(const SortOrder& rProvenance);

		Unit getDefaultUnit() const { return m_DefaultUnit; }
		void setDefaultUnit(Unit unit) { m_DefaultUnit = unit; }

	private:
		Ingredient(QString strName, const Category& rCategory, Unit defaultUnit);

		Ingredient(const Ingredient& rOther) = delete;
		void operator=(const Ingredient& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

	private:
		QString m_Name;
		const Category* m_pCategory;
		const SortOrder* m_pProvenance;
		Unit m_DefaultUnit;

		friend class RecipeBook;
	};
}

#endif
