#include "../include/TranslationManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QLocale>

TranslationManager::TranslationManager(QCoreApplication* app)
    : m_app(app)
{
}

void TranslationManager::initialize()
{
    loadLanguage("fr_FR");
}

bool TranslationManager::loadLanguage(const QString& langCode)
{
     QString qmFile = QCoreApplication::applicationDirPath() + "/i18n/" + langCode + ".qm";

    if (m_translator.load(qmFile)) {
        m_app->installTranslator(&m_translator);
        qDebug() << "Translation loaded:" << qmFile;
        return true;
    } else {
        qDebug() << "Failed to load translation:" << qmFile;
        return false;
    }
}