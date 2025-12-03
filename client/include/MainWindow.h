#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "NotConnectedLayout.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    NotConnectedLayout *layoutContainer;
};

#endif // MAINWINDOW_H