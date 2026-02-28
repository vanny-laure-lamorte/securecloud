#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "NotConnectedLayout.h"
#include "core/ClientService.h"

#include <QWidget>


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(ClientService* service, QWidget *parent = nullptr, TranslationManager* tm = nullptr);

private:
    NotConnectedLayout *layoutContainer;
    ClientService* service_;
    TranslationManager* translationManager_;
};

#endif // MAINWINDOW_H