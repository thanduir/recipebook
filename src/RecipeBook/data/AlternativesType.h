#ifndef RECIPEBOOK_ALTERNATIVESTYPE_H
#define RECIPEBOOK_ALTERNATIVESTYPE_H

#include <QColor>
#include <QString>

namespace recipebook
{
	class AlternativesType
	{
	public:
		QString getName() const { return m_Name; }
		QString getIdString() const { return getName(); }

		QColor getColor() const { return m_Color; }
		void setColor(QColor color) { m_Color = color; }

	private:
		explicit AlternativesType(QString strName) : m_Name(strName), m_Color(Qt::blue) {}      

		AlternativesType(const AlternativesType& rOther) = delete;
		void operator=(const AlternativesType& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

	private:
		QString m_Name;
		QColor m_Color;

		friend class RecipeBook;
	};
}

#endif
