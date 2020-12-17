#ifndef RECIPEBOOK_CATEGORY_H
#define RECIPEBOOK_CATEGORY_H

#include <QString>
#include "RBElementId.h"

namespace recipebook
{
	class Category
	{
	public:
		QString getName() const { return m_Name; }
		RBElementId getElementId() const { return getElementId(getName()); }

		bool operator!=(const Category& rOther) const { return !(*this == rOther); }
		bool operator==(const Category& rOther) const { return m_Name == rOther.m_Name; }

	private:
		explicit Category(QString strName) : m_Name(strName) {}      

		Category(const Category& rOther) = delete;
		void operator=(const Category& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

		static RBElementId getElementId(QString strName) { return RBElementId(strName); }

	private:
		QString m_Name;

		friend class RecipeBook;
	};
}

#endif
