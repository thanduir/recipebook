#include "Recipe.h"
#include "RecipeItem.h"
#include "Ingredient.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

Recipe::Recipe(QString strName, const Recipe& rOther)
:	m_Name(strName),
	m_NrPersons(rOther.getNumberOfPersons()),
	m_ShortDescription(rOther.getShortDescription()),
	m_RecipeText(rOther.getRecipeText()),
	m_CookingTime(rOther.getCookingTime())
{
	for(int i = 0; i < rOther.m_RecipeItems.size(); ++i)
	{
		const RecipeItem& rItem = rOther.getRecipeItemAt(i);
		internal::unsorted::addItem<RecipeItem>(rItem.getIdString(), i, m_RecipeItems, [&rItem]()
		{
			return new RecipeItem(rItem);
		});
	}
}

RecipeItem& Recipe::addRecipeItem(const Ingredient& rIngredient)
{
	return internal::unsorted::addItem<RecipeItem>(RecipeItem::getIdString(&rIngredient), -1, m_RecipeItems, [&rIngredient]()
	{
		return new RecipeItem(rIngredient);
	});
}

bool Recipe::existsRecipeItem(const Ingredient& rIngredient) const
{
	return internal::unsorted::exists<RecipeItem>(RecipeItem::getIdString(&rIngredient), m_RecipeItems);
}

bool Recipe::removeRecipeItem(const RecipeItem& rItem)
{
	internal::unsorted::remove(rItem.getIdString(), m_RecipeItems);
	return true;
}

RecipeItem& Recipe::getRecipeItem(const Ingredient& rIngredient)
{
	return internal::unsorted::getItem(RecipeItem::getIdString(&rIngredient), m_RecipeItems);
}

const RecipeItem& Recipe::getRecipeItem(const Ingredient& rIngredient) const
{
	return internal::unsorted::getItem(RecipeItem::getIdString(&rIngredient), m_RecipeItems);
}

quint32 Recipe::getRecipeItemsCount() const
{
	return m_RecipeItems.size();
}

RecipeItem& Recipe::getRecipeItemAt(quint32 i)
{
	if(i >= (quint32)m_RecipeItems.size())
	{
		throw QException();
	}
	return *m_RecipeItems.at(i).get();
}

const RecipeItem& Recipe::getRecipeItemAt(quint32 i) const
{
	if(i >= (quint32)m_RecipeItems.size())
	{
		throw QException();
	}
	return *m_RecipeItems.at(i).get();
}

void Recipe::moveRecipeItem(const RecipeItem& rItem, quint32 newPos)
{
	int oldPos = internal::unsorted::find(rItem.getIdString(), m_RecipeItems);
	if(oldPos < 0)
	{
		throw QException();
	}
	m_RecipeItems.move(oldPos, newPos);
}

quint32 Recipe::getRecipeItemIndex(QString strName) const
{
	return internal::unsorted::find(strName, m_RecipeItems);
}

bool Recipe::moveGroupItemsTogether()
{
	// TODO(phiwid): Refactor and symplify this emthod!

	bool bAlreadyCorrectlyOrdered = true;
	for(int i = 0; i < m_RecipeItems.count(); ++i)
	{
		QSharedPointer<RecipeItem> spItem = m_RecipeItems.at(i);
		if(!spItem->hasAlternativesGroup())
		{
			continue;
		}

		const AlternativesType& rCurrentGroup = spItem->getAlternativesGroup();

		// Find first element after group
		int firstAfterGroup = m_RecipeItems.count();
		for(int j = i + 1; j < m_RecipeItems.count(); ++j)
		{
			QSharedPointer<RecipeItem> spCurrentItem = m_RecipeItems.at(j);
			if(!spCurrentItem->hasAlternativesGroup() || &spCurrentItem->getAlternativesGroup() != &rCurrentGroup)
			{
				firstAfterGroup = j;
				break;
			}

			// Sort items in group alphabetically
			QSharedPointer<RecipeItem> spPrevItem = m_RecipeItems.at(j-1);
			if(recipebook::internal::helper::lessThan(spCurrentItem->getIdString(), spPrevItem->getIdString()))
			{
				moveRecipeItem(*spCurrentItem, j-1);
				bAlreadyCorrectlyOrdered = false;
			}
		}

		// Sort further group items
		for(int j = firstAfterGroup + 1; j < m_RecipeItems.count(); ++j)
		{
			QSharedPointer<RecipeItem> spCurrentItem = m_RecipeItems.at(j);
			if(spCurrentItem->hasAlternativesGroup() && &spCurrentItem->getAlternativesGroup() == &rCurrentGroup)
			{
				// Insert alphabetically
				int newPos = firstAfterGroup;
				for(int z = i; z < firstAfterGroup; ++z)
				{
					QSharedPointer<RecipeItem> spCurrentGroupItem = m_RecipeItems.at(z);
					if(recipebook::internal::helper::lessThan(spCurrentItem->getIdString(), spCurrentGroupItem->getIdString()))
					{
						newPos = z;
						break;
					}
				}
				moveRecipeItem(*spCurrentItem, newPos);

				firstAfterGroup += 1;
				bAlreadyCorrectlyOrdered = false;
			}
		}
	}

	return bAlreadyCorrectlyOrdered;
}
