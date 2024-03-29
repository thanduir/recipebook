#ifndef RECIPEBOOK_ALTERNATIVESTYPE_H
#define RECIPEBOOK_ALTERNATIVESTYPE_H

#include <QColor>
#include <QString>
#include "RBElementId.h"

namespace recipebook
{
	class AlternativesType
	{
	public:
		QString getName() const { return m_Name; }
		RBElementId getElementId() const { return getElementId(getName()); }

		QColor getColor() const { return m_Color; }
		void setColor(QColor color) { m_Color = color; }

		bool operator!=(const AlternativesType& rOther) const { return !(*this == rOther); }
		bool operator==(const AlternativesType& rOther) const { return m_Name == rOther.m_Name && m_Color == rOther.m_Color; }

	private:
		explicit AlternativesType(QString strName) : m_Name(strName), m_Color(Qt::blue) {}      

		AlternativesType(const AlternativesType& rOther) = delete;
		void operator=(const AlternativesType& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

		static RBElementId getElementId(QString strName) { return RBElementId(strName); }

	private:
		QString m_Name;
		QColor m_Color;

		friend class RecipeBook;
		friend class ShoppingListItem;
	};
}

#endif
