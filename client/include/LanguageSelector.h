#pragma once
#include <QWidget>
#include <QComboBox>
#include <QTranslator>
#include "TranslationManager.h"

class LanguageSelector : public QWidget
{
    Q_OBJECT
public:
    explicit LanguageSelector(QWidget* parent = nullptr);

signals:
    void languageChanged(const QString& lang);

private slots:

    /* Change application language and update flag icon
    * @param langCode (e.g., "en_US", "fr_FR" etc .)
    */
    void changeLanguageWithFlag(const QString &langCode);
    void onUserSelectedLanguage(int index);

private:

    TranslationManager* m_tm;
    QComboBox* combo;
    QTranslator translator;
};
