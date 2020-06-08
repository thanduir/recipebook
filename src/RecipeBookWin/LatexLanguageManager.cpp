#include "LatexLanguageManager.h"

using namespace recipebook;

LatexLanguageManager::LatexLanguageManager()
{
	m_LanguageNames.append(tr("English"));
	m_LanguageNames.append(tr("German"));
	m_LanguageNames.append(tr("French"));

	m_LanguageCodes.append("en");
	m_LanguageCodes.append("de");
	m_LanguageCodes.append("fr");

	m_LanguageLatexNames.append("");
	m_LanguageLatexNames.append("ngerman");
	m_LanguageLatexNames.append("french");
}

QString LatexLanguageManager::getBabelStringFromLangCode(QString code) const
{
	int index = m_LanguageCodes.indexOf(code);
	if(m_LanguageLatexNames[index].isEmpty())
	{
		return "";
	}

	return "\\usepackage[" + m_LanguageLatexNames[index] + "]{babel}\n";
}

QStringList LatexLanguageManager::availableLanguages() const
{
	return m_LanguageNames;
}

QString LatexLanguageManager::getLanguageCode(QString langName) const
{
	if(!m_LanguageNames.contains(langName))
	{
		return "en";
	}

	int index = m_LanguageNames.indexOf(langName);
	return m_LanguageCodes[index];
}

int LatexLanguageManager::getIndexFromLanguageCode(QString langCode) const
{
	if(!m_LanguageCodes.contains(langCode))
	{
		return 0;
	}

	return m_LanguageCodes.indexOf(langCode);
}
