#pragma once
#include <QWidget>
#include <QComboBox>
#include <QTranslator>

class LanguageSelector : public QWidget
{
    Q_OBJECT
public:
    explicit LanguageSelector(QWidget* parent = nullptr);

signals:
    void languageChanged(const QString& lang);

private slots:
    void onLanguageSelected(const QString& lang);

private:
    QComboBox* combo;
    QTranslator translator;
};
