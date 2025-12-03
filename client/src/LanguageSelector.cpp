#include "LanguageSelector.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QApplication>

LanguageSelector::LanguageSelector(QWidget* parent)
    : QWidget(parent)
{
    combo = new QComboBox(this);
    combo->setIconSize(QSize(24, 24));

    struct Language {
        QString name;
        QString code;
        QString iconPath;
    };

    QVector<Language> commonLanguages = {
        {"English", "EN", ":/assets/flags/en.png"},
        {"Français", "FR", ":/assets/flags/fr.png"},
        {"Español", "ES", ":/assets/flags/es.png"},
        {"Deutsch", "DE", ":/assets/flags/de.png"},
        {"Italiano", "IT", ":/assets/flags/it.png"}
    };

    for(const Language &lang : commonLanguages) {
        combo->addItem(QIcon(lang.iconPath),  QString("%1 ").arg(lang.code));
    }

    combo->setEditable(false);
    combo->setObjectName("language");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(combo);
    layout->setContentsMargins(0,0,0,0);

    connect(combo, &QComboBox::currentTextChanged, this, &LanguageSelector::onLanguageSelected);
}

void LanguageSelector::onLanguageSelected(const QString &code)
{
    QApplication::removeTranslator(&translator);
    (void)translator.load(":/translations/" + code + ".qm");
    QApplication::installTranslator(&translator);
    emit languageChanged(code);
}
