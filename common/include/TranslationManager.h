#pragma once
#include <QObject>
#include <QTranslator>
#include <QApplication>

class TranslationManager : public QObject {
    Q_OBJECT
public:
    explicit TranslationManager(QApplication* app, QObject* parent = nullptr);

    QString currentLanguage() const { return m_currentLang; }

    bool loadLanguage(const QString& langCode);
    QString getOsLanguage();

signals:
    void languageChanged(const QString& langCode);

private:
    QApplication* m_app;
    QTranslator m_translator;
    QString m_currentLang;
};
