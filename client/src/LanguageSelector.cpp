#include "LanguageSelector.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QApplication>

// TO DO: Delete when cout is not needed
using namespace std;
#include <iostream>

LanguageSelector::LanguageSelector(QWidget* parent)
    : QWidget(parent)
{
    QString osLang;
    if (m_tm) {
        osLang = m_tm->getOsLanguage();
    } else {
        osLang = "en_US";
        qWarning() << "m_tm is null!";
    }

    struct Language {
        QString name;
        QString shortCode;
        QString langCode;
        QString qmFilePath;
        QString iconPath;
    };

    QVector<Language> languages = {
        {"English", "EN", "en_US", ":/translations/en_US.qm", ":/assets/flags/en.png"},
        {"Français", "FR", "fr_FR", ":/translations/fr_FR.qm", ":/assets/flags/fr.png"},
        {"Español", "ES", "es_ES", ":/translations/es_ES.qm", ":/assets/flags/es.png"},
        {"Deutsch", "DE", "de_DE", ":/translations/de_DE.qm", ":/assets/flags/de.png"},
        {"Italiano", "IT", "it_IT", ":/translations/it_IT.qm", ":/assets/flags/it.png"}
    };

    combo = new QComboBox(this);
    combo->setIconSize(QSize(24, 24));

    int indexToSelect = 0;

    for(int i = 0; i < languages.size(); ++i) {
        const auto &lang = languages[i];

        QIcon icon(lang.iconPath);
        if (icon.isNull()) {
            qWarning() << "Icon not found:" << lang.iconPath;
        }
        combo->addItem(QIcon(lang.iconPath), lang.shortCode, lang.langCode);

        if (osLang.startsWith(lang.langCode.left(2))) {
            indexToSelect = i;
        }
    }

    combo->setEditable(false);
    combo->setCurrentIndex(indexToSelect);
    combo->setObjectName("language");
    combo->setMinimumWidth(80);
    combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(combo);
    layout->setContentsMargins(0, 0, 0, 0);

    // Connect signal for language change
    connect(combo, &QComboBox::currentIndexChanged, this, [this](int index){
        QString langCode = combo->itemData(index).toString();
        changeLanguageWithFlag(langCode);
    });
}
void LanguageSelector::onUserSelectedLanguage(int index)
{
    QString langCode = combo->itemData(index).toString();
    if (m_tm)
        m_tm->loadLanguage(langCode);
}

void LanguageSelector::changeLanguageWithFlag(const QString &langCode)
{
    int idx = combo->findData(langCode);

    if (idx >= 0 && combo->currentIndex() != idx)
        combo->setCurrentIndex(idx);
}