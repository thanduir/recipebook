#ifndef RECIPEBOOK_RECIPE_H
#define RECIPEBOOK_RECIPE_H

#include <QString>
#include <QSharedPointer>
#include <QTime>
#include "RecipeItemGroup.h"

namespace recipebook
{
    class Recipe
    {
    public:
        QString getName() const { return m_Name; }
        void rename(QString strNewName) { m_Name = strNewName; }

        quint32 getNumberOfPersons() const { return m_NrPersons; }
        void setNumberOfPersons(quint32 number) { m_NrPersons = number; }

        const RecipeItemGroup& getRecipeItems() const { return m_RecipeItems; }
        RecipeItemGroup& getRecipeItems() { return m_RecipeItems; }

        QString getShortDescription() const { return m_ShortDescription; }
        void setShortDescrpition(QString strText) { m_ShortDescription = strText; }

        QString getRecipeText() const { return m_RecipeText; }
        void setRecipeText(QString strText) { m_RecipeText = strText; }

        QTime getCookingTime() const { return m_CookingTime; }
        void setCookingTime(QTime duration) { m_CookingTime = duration; }

        // Alternatives groups

        RecipeItemGroup& addAlternativesGroup(QString strName);
        bool existsAlternativesGroup(QString strName) const;
        bool removeAlternativesGroup(const RecipeItemGroup& rGroup);
        RecipeItemGroup& getAlternativesGroup(QString strName);
        const RecipeItemGroup& getAlternativesGroup(QString strName) const;

        QStringList getAllAlternativesGroupsNamesSorted() const;

    private:
        explicit Recipe(QString strName, qint32 uiNrPersons) : m_NrPersons(uiNrPersons), m_Name(strName), m_CookingTime(0, 0) {}
        Recipe(QString strName, const Recipe& rOther);

        void operator=(const Recipe& rOther) = delete;

    private:
        QString m_Name;
        quint32 m_NrPersons;
        QString m_ShortDescription;
        QString m_RecipeText;
        QTime m_CookingTime;

        RecipeItemGroup m_RecipeItems = RecipeItemGroup("");
        QVector<QSharedPointer<RecipeItemGroup>> m_ItemGroups;

        friend class RecipeBook;
    };
}

#endif
