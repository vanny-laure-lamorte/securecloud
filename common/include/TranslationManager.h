#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QTranslator>
#include <QString>
#include <QApplication>

class TranslationManager {
public:
    explicit TranslationManager(QCoreApplication* app);

    // Initialize translation
    void initialize();

    // Loads a specific language manually
    bool loadLanguage(const QString& langCode);

private:
    QCoreApplication* m_app;
    QTranslator m_translator;
};

#endif // TRANSLATIONMANAGER_H
