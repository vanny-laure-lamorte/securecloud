#pragma once
#include <QWidget>
#include <QComboBox>
#include <QTranslator>
#include "TranslationManager.h"

class LanguageSelector : public QWidget
{
    Q_OBJECT
public:
    explicit LanguageSelector(QWidget* parent = nullptr, TranslationManager* tm = nullptr);

// signals:

//     void languageChanged(const QString& lang);

private:

    TranslationManager* translationManager_;
    QComboBox* combo;
    QTranslator translator;
};
