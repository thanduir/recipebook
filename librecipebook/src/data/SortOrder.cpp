#include "data/SortOrder.h"
#include <QSharedPointer>
#include <QException>
#include "data/Category.h"

recipebook::SortOrder::SortOrder(QString strName, const QVector<QSharedPointer<Category>>& rCategories)
:	m_Name(strName)
{
	for(QSharedPointer<Category> spCategory : qAsConst(rCategories))
	{
		m_Categories.append(spCategory.get());
	}
}

const recipebook::Category& recipebook::SortOrder::getItemAt(quint32 i) const
{
	if(i >= static_cast<quint32>(m_Categories.size()))
	{
		throw QException();
	}

    return *m_Categories.at((int)i);
}

void recipebook::SortOrder::moveCategory(const Category& category, quint32 newPos)
{
    int oldPos = m_Categories.indexOf(&category);
    m_Categories.move(oldPos, (int)newPos);
}

quint32 recipebook::SortOrder::getIndex(const Category& category) const
{
	for(quint32 i = 0; i < (quint32)m_Categories.size(); ++i)
	{
        if(m_Categories.at((int)i) == &category)
		{
			return i;
		}
	}

	return 0;
}

bool recipebook::SortOrder::operator==(const SortOrder& rOther) const
{
	if(m_Name != rOther.m_Name)
	{
		return false;
	}

	if(m_Categories.size() != rOther.m_Categories.size())
	{
		return false;
	}

	for(int i = 0; i < m_Categories.size(); ++i)
	{
		const Category* pCategory1 = m_Categories.at(i);
		const Category* pCategory2 = rOther.m_Categories.at(i);
		if(*pCategory1 != *pCategory2)
		{
			return false;
		}
	}

	return true;
}
