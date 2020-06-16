#include "data/RecipeBookConfigItem.h"
#include "data/Recipe.h"


using namespace recipebook;

RBElementId RecipeBookConfigItem::getElementId(RecipeBookConfigItemType type, QString strElementIdBase)
{
	switch(type)
	{
		case RecipeBookConfigItemType::Header:
		{
			return RBElementId("Header_" + strElementIdBase);
		}

		case RecipeBookConfigItemType::Recipe:
		{
			return RBElementId("Recipe_" + strElementIdBase);
		}
	}

	return RBElementId("");
}

RecipeBookConfigItem::RecipeBookConfigItem(const Recipe& rRecipe)
:	m_Type(RecipeBookConfigItemType::Recipe),
	m_pRecipe(&rRecipe),
	m_HeaderName(),
	m_uiLevel(-1)
{
}

RecipeBookConfigItem::RecipeBookConfigItem(QString strHeaderName, QString strElementIdBase, qint32 uiLevel)
:	m_Type(RecipeBookConfigItemType::Header),
	m_pRecipe(nullptr),
	m_HeaderName(strHeaderName),
	m_HeaderIdBase(strElementIdBase),
	m_uiLevel(uiLevel)
{
}

RecipeBookConfigItem::RecipeBookConfigItem(const RecipeBookConfigItem& rOther)
:	m_Type(rOther.m_Type),
	m_pRecipe(rOther.m_pRecipe),
	m_HeaderName(rOther.m_HeaderName),
	m_uiLevel(rOther.m_uiLevel)
{
}

QString RecipeBookConfigItem::getName() const
{
	switch(m_Type)
	{
		case RecipeBookConfigItemType::Header:
		{
			return m_HeaderName;
		}

		case RecipeBookConfigItemType::Recipe:
		{
			return m_pRecipe->getName();
		}
	}

	return "";
}

QString RecipeBookConfigItem::getElementIdBase() const
{
	if(m_Type == RecipeBookConfigItemType::Header)
	{
		return m_HeaderIdBase;
	}

	return getName();
}

bool RecipeBookConfigItem::rename(QString newName)
{
	if(m_Type != RecipeBookConfigItemType::Header)
	{
		return false;
	}

	m_HeaderName = newName;
	return true;
}
