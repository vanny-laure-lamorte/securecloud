#ifndef HEADER_H
#define HEADER_H

#include <QWidget>

class QLabel;
class QToolButton;
class QMenu;

class Header : public QWidget
{
    Q_OBJECT
public:
    explicit Header(QWidget *parent = nullptr);

private:
    QLabel* logoLabel;
    QLabel* titleLabel;
    QToolButton* menuButton;
    QMenu* dropdownMenu;

};

#endif // HEADER_H
