#include "RecipeBookConfiguration.h"
#include <QException>
#include "RecipeBookConfigItem.h"
#include "RecipeBook.h"
#include "Recipe.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

RecipeBookConfiguration::RecipeBookConfiguration(QString strName, const RecipeBook* pRecipeBook)
:	m_Name(strName),
	m_Title(strName)
{
	if(pRecipeBook != nullptr)
	{
		for(quint32 i = 0; i < pRecipeBook->getRecipesCount(); ++i)
		{
			addRecipe(pRecipeBook->getRecipeAt(i));
		}
	}
}

RecipeBookConfiguration::RecipeBookConfiguration(QString strName, const RecipeBookConfiguration& rOther)
:	m_Name(strName),
	m_Title(rOther.getBookTitle()),
	m_Subtitle(rOther.getBookSubtitle()),
	m_FontSize(rOther.getFontSize()),
	m_LangCode(rOther.getLanguageCode())
{
	for(quint32 i = 0; i < rOther.getItemsCount(); ++i)
	{
		const RecipeBookConfigItem& rItem = rOther.getItemAt(i);
		internal::unsorted::addItem<RecipeBookConfigItem>(rItem.getElementId(), -1, m_Items, [&rItem]()
		{
			return new RecipeBookConfigItem(rItem);
		});
	}
}

RecipeBookConfigItem& RecipeBookConfiguration::addRecipe(const Recipe& rRecipe, qint32 pos)
{
	RBElementId idString = RecipeBookConfigItem::getElementId(RecipeBookConfigItemType::Recipe, rRecipe.getName());
	return internal::unsorted::addItem<RecipeBookConfigItem>(idString, pos, m_Items, [&rRecipe]()
	{
		return new RecipeBookConfigItem(rRecipe);
	});
}

RecipeBookConfigItem& RecipeBookConfiguration::addHeader(QString strName, quint32 uiLevel, qint32 pos)
{
	// Find unsed id
	QString idStringBase = strName;
	RBElementId idString = RecipeBookConfigItem::getElementId(RecipeBookConfigItemType::Header, idStringBase);
	int i = 1;
	while(internal::unsorted::exists(idString, m_Items))
	{
		idStringBase = QString("%1.%2").arg(strName).arg(i);
		idString = RecipeBookConfigItem::getElementId(RecipeBookConfigItemType::Header, idStringBase);
		++i;
	}

	return internal::unsorted::addItem<RecipeBookConfigItem>(idString, pos, m_Items, [strName, idStringBase, uiLevel]()
	{
		return new RecipeBookConfigItem(strName, idStringBase, uiLevel);
	});
}

bool RecipeBookConfiguration::existsRecipe(const Recipe& rRecipe) const
{
	RBElementId idString = RecipeBookConfigItem::getElementId(RecipeBookConfigItemType::Recipe, rRecipe.getName());
	return internal::unsorted::exists<RecipeBookConfigItem>(idString, m_Items);
}

bool RecipeBookConfiguration::removeItem(quint32 pos)
{
	if(pos >= (quint32)m_Items.count())
	{
		return false;
	}

	m_Items.remove(pos);
	return true;
}

bool RecipeBookConfiguration::removeRecipe(const Recipe& rRecipe)
{
	RBElementId idString = RecipeBookConfigItem::getElementId(RecipeBookConfigItemType::Recipe, rRecipe.getName());
	internal::unsorted::remove(idString, m_Items);
	return true;
}

quint32 RecipeBookConfiguration::getItemsCount() const
{
	return m_Items.size();
}

RecipeBookConfigItem& RecipeBookConfiguration::getItemAt(quint32 i)
{
	if(i >= (quint32)m_Items.size())
	{
		throw QException();
	}
	return *m_Items.at(i).get();
}

const RecipeBookConfigItem& RecipeBookConfiguration::getItemAt(quint32 i) const
{
	if(i >= (quint32)m_Items.size())
	{
		throw QException();
	}
	return *m_Items.at(i).get();
}

void RecipeBookConfiguration::moveItem(const RecipeBookConfigItem& rItem, quint32 newPos)
{
	int oldPos = internal::unsorted::find(rItem.getElementId(), m_Items);
	if(oldPos < 0)
	{
		throw QException();
	}
	m_Items.move(oldPos, newPos);
}
