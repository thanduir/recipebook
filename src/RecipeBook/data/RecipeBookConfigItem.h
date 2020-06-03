#ifndef RECIPEBOOK_CONFIG_ITEM_H
#define RECIPEBOOK_CONFIG_ITEM_H

#include <QString>
#include "util/RBElementId.h"

namespace recipebook
{
	class Recipe;

	enum class RecipeBookConfigItemType
	{
		Recipe,
		Header
	};

	class RecipeBookConfigItem
	{
	public:
		QString getName() const;
		bool rename(QString newName);

		RBElementId getElementId() const { return getElementId(m_Type, getElementIdBase()); }
		RecipeBookConfigItemType getType() const { return m_Type; }

		const Recipe* getRecipe() const { return m_pRecipe; }
		
		qint32 getLevel() const { return m_uiLevel; }
		void setLevel(qint32 uiLevel) { m_uiLevel = uiLevel; }

	private:
		explicit RecipeBookConfigItem(const Recipe& rRecipe);
		RecipeBookConfigItem(QString strHeaderName, QString strElementIdBase, quint32 uiLevel);
		RecipeBookConfigItem(const RecipeBookConfigItem& rOther);

		void operator=(const RecipeBookConfigItem& rOther) = delete;

		QString getElementIdBase() const;
		static RBElementId getElementId(RecipeBookConfigItemType type, QString strElementIdBase);

	private:
		RecipeBookConfigItemType	m_Type;
		const Recipe*				m_pRecipe = nullptr;
		QString						m_HeaderName;
		QString						m_HeaderIdBase;
		qint32						m_uiLevel = -1;

		friend class RecipeBookConfiguration;
	};
}

#endif
