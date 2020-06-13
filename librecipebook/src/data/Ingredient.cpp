#include "data/Ingredient.h"
#include <QException>

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
