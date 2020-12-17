#include "data/Ingredient.h"
#include <QException>
#include "data/Category.h"
#include "data/SortOrder.h"

recipebook::Ingredient::Ingredient(QString strName, const Category& rCategory, Unit defaultUnit)
:	m_Name(strName),
	m_pCategory(&rCategory),
	m_DefaultUnit(defaultUnit)
{
}

bool recipebook::Ingredient::provenanceAvailable(const SortOrder& rProvenance) const
{
	for(const SortOrder* pOrder : qAsConst(m_UnavailableProvenances))
	{
		if(pOrder == &rProvenance)
		{
			return false;
		}
	}
	return true;
}

void recipebook::Ingredient::setProvenanceAvailable(const SortOrder& rProvenance)
{
	for(const SortOrder* pOrder : qAsConst(m_UnavailableProvenances))
	{
		if(pOrder == &rProvenance)
		{
			m_UnavailableProvenances.removeAll(&rProvenance);
			return;
		}
	}
}

void recipebook::Ingredient::setProvenanceUnavailable(const SortOrder& rProvenance)
{
	for(const SortOrder* pOrder : qAsConst(m_UnavailableProvenances))
	{
		if(pOrder == &rProvenance)
		{
			// Already unavailable
			return;
		}
	}
	m_UnavailableProvenances.append(&rProvenance);
}

bool recipebook::Ingredient::operator==(const Ingredient& rOther) const
{
	if(m_Name != rOther.m_Name)
	{
		return false;
	}

	if(*m_pCategory != *rOther.m_pCategory)
	{
		return false;
	}
	
	if(m_DefaultUnit != rOther.m_DefaultUnit)
	{
		return false;
	}

	if(m_UnavailableProvenances.size() != rOther.m_UnavailableProvenances.size())
	{
		return false;
	}

	for(const SortOrder* pOrder1 : m_UnavailableProvenances)
	{
		bool bFound = false;
		for(const SortOrder* pOrder2 : m_UnavailableProvenances)
		{
			if(*pOrder1 == *pOrder2)
			{
				bFound = true;
				break;
			}
		}

		if(!bFound)
		{
			return false;
		}
	}

	return true;
}
