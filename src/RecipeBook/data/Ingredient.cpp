#include "Ingredient.h"
#include <QException>

recipebook::Ingredient::Ingredient(QString strName, const Category& rCategory, Unit defaultUnit)
:	m_Name(strName),
	m_pCategory(&rCategory),
	m_bProvenanceEverywhere(true),
	m_pProvenance(nullptr),
	m_DefaultUnit(defaultUnit)
{
}

const recipebook::SortOrder& recipebook::Ingredient::getProvenance() const
{
	if(m_bProvenanceEverywhere)
	{
		throw QException();
	}

	return *m_pProvenance;
}

void recipebook::Ingredient::setProvenanceEverywhere()
{
	m_bProvenanceEverywhere = true;
	m_pProvenance = nullptr;
}

void recipebook::Ingredient::setProvenance(const SortOrder& rProvenance)
{
	m_bProvenanceEverywhere = false;
	m_pProvenance = &rProvenance;
}
