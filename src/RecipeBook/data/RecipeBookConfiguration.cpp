#include "RecipeBookConfiguration.h"
#include <QException>
#include "RecipeBookConfigItem.h"
#include "RecipeBook.h"
#include "Recipe.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

RecipeBookConfiguration::RecipeBookConfiguration(QString strName, const RecipeBook& rRecipeBook)
:	m_Name(strName),
	m_Title(strName)
{
	for(quint32 i = 0; i < rRecipeBook.getRecipesCount(); ++i)
	{
		addRecipe(rRecipeBook.getRecipeAt(i));
	}
}

RecipeBookConfiguration::RecipeBookConfiguration(QString strName, const RecipeBookConfiguration& rOther)
:	m_Name(strName),
	m_Title(rOther.getBookTitle()),
	m_Subtitle(rOther.getBookSubtitle()),
	m_FontSize(rOther.getFontSize())
{
	for(quint32 i = 0; i < rOther.getItemsCount(); ++i)
	{
		const RecipeBookConfigItem& rItem = rOther.getItemAt(i);
		internal::unsorted::addItem<RecipeBookConfigItem>(rItem.getIdString(), -1, m_Items, [&rItem]()
		{
			return new RecipeBookConfigItem(rItem);
		});
	}
}

RecipeBookConfigItem& RecipeBookConfiguration::addRecipe(const Recipe& rRecipe, qint32 pos)
{
	QString idString = RecipeBookConfigItem::getIdString(RecipeBookConfigItemType::Recipe, rRecipe.getName());
	return internal::unsorted::addItem<RecipeBookConfigItem>(idString, pos, m_Items, [&rRecipe]()
	{
		return new RecipeBookConfigItem(rRecipe);
	});
}

RecipeBookConfigItem& RecipeBookConfiguration::addHeader(QString strName, quint32 uiLevel, qint32 pos)
{
	QString idString = RecipeBookConfigItem::getIdString(RecipeBookConfigItemType::Header, strName);
	return internal::unsorted::addItem<RecipeBookConfigItem>(idString, pos, m_Items, [strName, uiLevel]()
	{
		return new RecipeBookConfigItem(strName, uiLevel);
	});
}

bool RecipeBookConfiguration::existsRecipe(const Recipe& rRecipe) const
{
	QString idString = RecipeBookConfigItem::getIdString(RecipeBookConfigItemType::Recipe, rRecipe.getName());
	return internal::unsorted::exists<RecipeBookConfigItem>(idString, m_Items);
}

bool RecipeBookConfiguration::existsHeader(QString strName) const
{
	QString idString = RecipeBookConfigItem::getIdString(RecipeBookConfigItemType::Header, strName);
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
	QString idString = RecipeBookConfigItem::getIdString(RecipeBookConfigItemType::Recipe, rRecipe.getName());
	internal::unsorted::remove(idString, m_Items);
	return true;
}

bool RecipeBookConfiguration::removeHeader(QString strName)
{
	QString idString = RecipeBookConfigItem::getIdString(RecipeBookConfigItemType::Header, strName);
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

void RecipeBookConfiguration::moveRecipeItem(const RecipeBookConfigItem& rItem, quint32 newPos)
{
	int oldPos = internal::unsorted::find(rItem.getIdString(), m_Items);
	if(oldPos < 0)
	{
		throw QException();
	}
	m_Items.move(oldPos, newPos);
}

quint32 RecipeBookConfiguration::getItemIndex(QString idString) const
{
	return internal::unsorted::find(idString, m_Items);
}
