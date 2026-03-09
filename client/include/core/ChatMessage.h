#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>

struct ChatMessage
{
    int senderId;
    QString senderName;
    QString content;
};

#endif