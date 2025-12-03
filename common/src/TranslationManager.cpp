#include "../include/TranslationManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QLocale>

#ifdef _WIN32
#define RPC_NO_WINDOWS_H
#include <windows.h>
#endif

TranslationManager::TranslationManager(QCoreApplication* app)
    : m_app(app)
{
}

void TranslationManager::initialize()
{
    // Get system language
    QString langCode;
    #ifdef _WIN32
        LANGID langId = GetUserDefaultUILanguage();

        wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = {0};
        if (LCIDToLocaleName(MAKELCID(langId, SORT_DEFAULT), localeName, LOCALE_NAME_MAX_LENGTH, 0))
        {
            langCode = QString::fromWCharArray(localeName);
            langCode.replace("-", "_");
        }
        else
        {
            langCode = "en_US";
        }
    #else
        // langCode = QLocale::system().uiLanguages().first().replace("-", "_");
    #endif
    qDebug() << "User language :" << langCode;
    if (!loadLanguage(langCode))
    {
        loadLanguage("en_US");
    }
}

bool TranslationManager::loadLanguage(const QString& langCode)
{
    QString qmFile = QCoreApplication::applicationDirPath() + "/i18n/" + langCode + ".qm";
    m_app->removeTranslator(&m_translator);

    if (m_translator.load(qmFile))
    {
        m_app->installTranslator(&m_translator);
        qDebug() << "Traduction chargée :" << qmFile;
        return true;
    }
    else
    {
        qDebug() << "Impossible de charger la traduction :" << qmFile;
        return false;
    }
}