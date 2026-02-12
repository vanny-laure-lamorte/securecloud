#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "core/ClientService.h"

class QTextEdit;
class QLineEdit;

class ChatPage : public QWidget
{
    Q_OBJECT
public:
    explicit ChatPage(ClientService *service, QWidget *parent = nullptr);

private:
    QTextEdit* chatView;
    QLineEdit* messageInput;

};

#endif // CHATPAGE_H
