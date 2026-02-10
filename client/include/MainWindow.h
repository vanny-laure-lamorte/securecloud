#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "NotConnectedLayout.h"
#include "core/ClientService.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(ClientService* service, QWidget *parent = nullptr);

private:
    NotConnectedLayout *layoutContainer;
    ClientService* service_;
};

#endif // MAINWINDOW_H