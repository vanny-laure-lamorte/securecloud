#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "core/ClientService.h"

class QLabel;
class QLineEdit;
class QVBoxLayout;
class QScrollArea;
class QWidget;

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(ClientService *service, QWidget *parent = nullptr);
    void loadConversation(int chatId, const QString& conversationName, const QString& type = "personal");
    void setConversationType(const QString& type) { conversationType = type.toStdString(); }

private:
    void clearMessages();
    void addMessage(int senderId, const QString& senderName, const QString& content);

    ClientService* service;
    int activeChatId = -1;
    std::string conversationType = "group";

    QLabel* userNameLabel;
    QLineEdit* messageInput;

    QScrollArea* messagesScrollArea;
    QWidget* messagesContainer;
    QVBoxLayout* messagesLayout;
};

#endif // CHATPAGE_H