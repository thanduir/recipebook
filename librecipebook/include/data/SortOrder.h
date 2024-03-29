#ifndef RECIPEBOOK_SORTORDER_H
#define RECIPEBOOK_SORTORDER_H

#include <QList>
#include <QVector>
#include <QSharedPointer>
#include "RBElementId.h"

namespace recipebook
{
	class Category;

	class SortOrder
	{
	public:
		QString getName() const { return m_Name; }
		RBElementId getElementId() const { return getElementId(getName()); }

        quint32 getItemsCount() const { return (quint32)m_Categories.size(); }
		const Category& getItemAt(quint32 i) const;

		void moveCategory(const Category& category, quint32 newPos);

		quint32 getIndex(const Category& category) const;

		bool operator!=(const SortOrder& rOther) const { return !(*this == rOther); }
		bool operator==(const SortOrder& rOther) const;

	private:
		SortOrder(QString strName, const QVector<QSharedPointer<Category>>& rCategories);

		SortOrder(const SortOrder& rOther) = delete;
		void operator=(const SortOrder& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

		static RBElementId getElementId(QString strName) { return RBElementId(strName); }

	private:
		QString m_Name;
		QList<const Category*> m_Categories;

		friend class RecipeBook;
	};
}

#endif
