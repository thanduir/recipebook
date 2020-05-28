#ifndef RECIPEBOOK_CONFIGURATION_H
#define RECIPEBOOK_CONFIGURATION_H

#include <QVector>
#include <QSharedPointer>

namespace recipebook
{
	class RecipeBook;
	class Recipe;
	class RecipeBookConfigItem;

	class RecipeBookConfiguration
	{
	public:
		QString getName() const { return m_Name; }
		QString getIdString() const { return getName(); }
        
		QString getBookTitle() const { return m_Title; }
		void setBookTitle(QString strTitle) { m_Title = strTitle; }

		QString getBookSubtitle() const { return m_Subtitle; }
		void setBookSubtitle(QString strSubtitle) { m_Subtitle = strSubtitle; }

		quint32 getFontSize() const { return m_FontSize; }
		void setFontSize(quint32 fontSize) { m_FontSize = fontSize; }

		// Items

		RecipeBookConfigItem& addRecipe(const Recipe& rRecipe, qint32 pos = -1);
		RecipeBookConfigItem& addHeader(QString strName, quint32 uiLevel = 0, qint32 pos = -1);

		bool existsRecipe(const Recipe& rRecipe) const;
		bool existsHeader(QString strName) const;

		bool removeItem(quint32 pos);
		bool removeRecipe(const Recipe& rRecipe);
		bool removeHeader(QString strName);

		quint32 getItemsCount() const;
		RecipeBookConfigItem& getItemAt(quint32 i);
		const RecipeBookConfigItem& getItemAt(quint32 i) const;
		void moveRecipeItem(const RecipeBookConfigItem& rItem, quint32 newPos);

		quint32 getItemIndex(QString idString) const;

	private:
		RecipeBookConfiguration(QString strName, const RecipeBook& rRecipeBook);
		RecipeBookConfiguration(QString strName, const RecipeBookConfiguration& rOther);
		// TODO: Second ctor for loading from file? Or how to do that?
		
		void operator=(const RecipeBookConfiguration& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

	private:
		QString m_Name;
		QString m_Title;
		QString m_Subtitle;
		quint32 m_FontSize = 10;

		QVector<QSharedPointer<RecipeBookConfigItem>> m_Items;

		friend class RecipeBook;
	};
}

#endif
