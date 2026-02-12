#include "../include/TranslationManager.h"
#include <QDebug>
#include <QLocale>

#ifdef _WIN32
#define RPC_NO_WINDOWS_H
#include <windows.h>
#endif

#include <iostream>
using namespace std;

TranslationManager::TranslationManager(QApplication* app, QObject* parent)
    : QObject(parent), m_app(app)
{

}

bool TranslationManager::loadLanguage(const QString& langCode)
{
    // TODO: Get user's language preference from settings and load that instead of OS language
    QString qmFile = QCoreApplication::applicationDirPath() + "/i18n/" + langCode + ".qm";

    m_app->removeTranslator(&m_translator);

    if (m_translator.load(qmFile))
    {
        m_app->installTranslator(&m_translator);
        m_currentLang = langCode;
        qDebug() << "File loaded for traduction :" << qmFile;
        emit languageChanged(langCode);
        return true;
    }
    else
    {
        qDebug() << "File not loaded" << qmFile;
        return false;
    }
}

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
        langCode = "en_US";
    }
#else
    langCode = QLocale::system().uiLanguages().first().replace("-", "_");
#endif

    return langCode;
}