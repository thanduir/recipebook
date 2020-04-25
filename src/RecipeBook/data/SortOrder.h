#ifndef RECIPEBOOK_SORTORDER_H
#define RECIPEBOOK_SORTORDER_H

#include <QList>

namespace recipebook
{
	class Category;

	class SortOrder
	{
	public:
		QString getName() const { return m_Name; }

		quint32 getItemsCount() const { return m_Categories.size(); }
		const Category& at(quint32 i) const;

		void moveCategory(const Category& category, quint32 newPos);

		quint32 getIndex(const Category& category) const;

	private:
		SortOrder(QString strName, const QVector<QSharedPointer<Category>>& rCategories);

		SortOrder(const SortOrder& rOther) = delete;
		void operator=(const SortOrder& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

	private:
		QString m_Name;
		QList<const Category*> m_Categories;

		friend class RecipeBook;
	};
}

#endif
