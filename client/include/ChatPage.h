#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>

class QTextEdit;
class QLineEdit;

class ChatPage : public QWidget
{
    Q_OBJECT
public:
    explicit ChatPage(QWidget *parent = nullptr);

private:
    QTextEdit* chatView;
    QLineEdit* messageInput;

};

#endif // CHATPAGE_H
