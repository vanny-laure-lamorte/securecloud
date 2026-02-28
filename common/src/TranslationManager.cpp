#include "../include/TranslationManager.h"
#include <QDebug>
#include <QLocale>

#ifdef _WIN32
#define RPC_NO_WINDOWS_H
#include <windows.h>
#endif

#include <iostream>
using namespace std;

TranslationManager::TranslationManager(QApplication *app, QObject *parent)
    : QObject(parent), m_app(app) {}

QString TranslationManager::getOsLanguage()
{
    QString langCode;

#ifdef _WIN32
    LANGID langId = GetUserDefaultUILanguage();
    wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = {0};
    if (LCIDToLocaleName(MAKELCID(langId, SORT_DEFAULT), localeName, LOCALE_NAME_MAX_LENGTH, 0))
    {
        langCode = QString::fromWCharArray(localeName).replace("-", "_");
    }
    else
    {
        langCode = "fr_FR";
    }
#else
    langCode = QLocale::system().uiLanguages().first().replace("-", "_");
#endif

    if (langCode.isEmpty())
    {
        langCode = "fr_fR";
        qWarning() << "No language available, defaulting to" << langCode;
    }
    return langCode;
}

bool TranslationManager::loadLanguage(const QString &langCode)
{
    m_app->removeTranslator(&m_translator);

    QString qmFile = QCoreApplication::applicationDirPath() + "/i18n/" + langCode + ".qm";

    if (m_translator.load(qmFile))
    {
        if (!m_app->installTranslator(&m_translator))
        {
            return false;
        }
        m_currentLang = langCode;
        emit languageChanged(langCode);
        return true;
    }
    else
    {
        return false;
    }
}