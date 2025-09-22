#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QTranslator>
#include <QString>
#include <QApplication>

class TranslationManager {
public:
    TranslationManager(QApplication* app);

    // Initialize translation
    void initialize();

    // Loads a specific language manually
    bool loadLanguage(const QString& langCode);

private:
    QApplication* m_app;
    QTranslator m_translator;
};

#endif // TRANSLATIONMANAGER_H
