#include <QSharedPointer>
#include <QException>
#include "SortOrder.h"
#include "Category.h"

recipebook::SortOrder::SortOrder(QString strName, const QVector<QSharedPointer<Category>>& rCategories)
:	m_Name(strName)
{
	for(QSharedPointer<Category> spCategory : qAsConst(rCategories))
	{
		m_Categories.append(spCategory.get());
	}
}

const recipebook::Category& recipebook::SortOrder::at(quint32 i) const
{
	if(i >= static_cast<quint32>(m_Categories.size()))
	{
		throw QException();
	}

	return *m_Categories.at(i);
}

void recipebook::SortOrder::moveCategory(const Category& category, quint32 newPos)
{
	quint32 oldPos = m_Categories.indexOf(&category);
	m_Categories.move(oldPos, newPos);
}
