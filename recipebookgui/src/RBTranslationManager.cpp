#include "RBTranslationManager.h"
#include <QApplication>
#include <QDir>
#include "RecipeBookSettings.h"

constexpr const char* c_langFolder              = ":/translations";
constexpr const char* c_langFolderQt			= "/translations";
constexpr const char* c_LanguageFileBaseName    = "recipebookgui_%1.qm";

using namespace recipebook::UI;

namespace
{
	bool switchTranslator(QTranslator& translator, bool bQt, const QString& filename)
	{
		qApp->removeTranslator(&translator);

		QString filePath;
		if(bQt)
		{
			filePath = QApplication::applicationDirPath();
			filePath.append(c_langFolderQt);
			filePath.append("/").append(filename);
		}
		else
		{
			filePath = c_langFolder;
			filePath.append("/").append(filename);
		}

		if(translator.load(filePath))
		{
			qApp->installTranslator(&translator);
			return true;
		}

		return false;
	}
}

RBTranslationManager::RBTranslationManager(RecipeBookSettings& rSettings)
:	m_rSettings(rSettings),
	m_pEngine(nullptr)
{
	generateLanguagesList();
	QString lang = getCurrentLanguage();
	switchCurrentLanguage(lang);
}

void RBTranslationManager::setQmlEngine(QQmlApplicationEngine& rEngine)
{
	m_pEngine = &rEngine;
}

void RBTranslationManager::generateLanguagesList()
{
	QDir dir(c_langFolder);
	QStringList filenames = dir.entryList(QStringList(QString(c_LanguageFileBaseName).arg("*")));

	m_AvailableLanguages.clear();
	for(QString filename : filenames) 
	{
		filename.truncate(filename.lastIndexOf('.')); // "recipebookwin_de"
		filename.remove(0, filename.indexOf('_') + 1); // "de"
		m_AvailableLanguages.append(filename);
	}
}

QString RBTranslationManager::getCurrentLanguage() const
{
	QString lang = m_rSettings.getCurrentAppLanguage();
	if(!lang.isEmpty() && m_AvailableLanguages.contains(lang))
	{
		return lang;
	}

	QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
	defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
	return defaultLocale;
}

QString RBTranslationManager::getCurrentLanguageName() const
{
	QString lang = getCurrentLanguage();
	if(m_AvailableLanguages.contains(lang))
	{
        qsizetype index = m_AvailableLanguages.indexOf(lang);
		return m_NamesOfAvailableLanguages[index];
	}
	
	return QLocale::languageToString(QLocale(lang).language());
}

QString RBTranslationManager::getCurrentLanguageCode() const
{
	return getCurrentLanguage();
}

void RBTranslationManager::setCurrentLanguageIndex(int index)
{
	if(index >= m_AvailableLanguages.size())
	{
		return;
	}

	switchCurrentLanguage(m_AvailableLanguages[index]);

	if(m_pEngine != nullptr)
	{
		m_pEngine->retranslate();
	}
}

void RBTranslationManager::switchCurrentLanguage(QString lang)
{
	QLocale locale = QLocale(lang);
	QLocale::setDefault(locale);
	QString languageName = QLocale::languageToString(locale.language());
	switchTranslator(m_translator, false, QString(c_LanguageFileBaseName).arg(lang));
	switchTranslator(m_translatorQt, true, QString("qt_%1.qm").arg(lang));

	m_rSettings.setCurrentAppLanguage(lang);

	generateLanguageNames();
}

void RBTranslationManager::generateLanguageNames()
{
	m_NamesOfAvailableLanguages.clear();
	for(QString langName : m_AvailableLanguages)
	{
		QString langStr = QLocale::languageToString(QLocale(langName).language());
		m_NamesOfAvailableLanguages.append(langStr);
	}
}
