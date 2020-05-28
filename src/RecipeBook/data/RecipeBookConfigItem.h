#ifndef RECIPEBOOK_CONFIG_ITEM_H
#define RECIPEBOOK_CONFIG_ITEM_H

#include <QString>

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
		static QString getIdString(RecipeBookConfigItemType type, QString strName);

		QString getName() const;
		QString getIdString() const { return getIdString(m_Type, getName()); }
		RecipeBookConfigItemType getType() const { return m_Type; }

		const Recipe* getRecipe() const { return m_pRecipe; }
		
		qint32 getLevel() const { return m_uiLevel; }
		void setLevel(qint32 uiLevel) { m_uiLevel = uiLevel; }

	private:
		explicit RecipeBookConfigItem(const Recipe& rRecipe);
		RecipeBookConfigItem(QString strHeaderName, quint32 uiLevel);
		RecipeBookConfigItem(const RecipeBookConfigItem& rOther);

		void operator=(const RecipeBookConfigItem& rOther) = delete;

	private:
		RecipeBookConfigItemType	m_Type;
		const Recipe*				m_pRecipe = nullptr;
		QString						m_HeaderName;
		qint32						m_uiLevel = -1;

		friend class RecipeBookConfiguration;
	};
}

#endif
