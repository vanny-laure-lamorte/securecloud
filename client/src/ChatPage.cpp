#include "ChatPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTabBar>
#include <QFrame>
#include <QToolButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QLayoutItem>
#include <QIcon>

ChatPage::ChatPage(ClientService* service, QWidget *parent)
    : QWidget(parent), service(service)
{
    this->setStyleSheet("background-color:#ffffff; color:#017f6a;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // --- Header ---
    QFrame* header = new QFrame(this);
    header->setStyleSheet("background-color:white;");
    header->setFixedHeight(60);

    QHBoxLayout* headerLayout = new QHBoxLayout(header);

    userNameLabel = new QLabel("Jean Dupont", header);
    userNameLabel->setStyleSheet("font-size:18px; font-weight:bold; color:black;");
    headerLayout->addWidget(userNameLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(header);

    // --- Tabs ---
    QTabBar* tabs = new QTabBar(this);
    tabs->addTab("Conversation");
    tabs->addTab("Fichiers");
    tabs->addTab("Photos");
    tabs->setStyleSheet(R"(
        QTabBar {
            background:#1e1e1e;
        }
        QTabBar::tab {
            background:#e8fff7;
            padding:10px;
            border:2px solid #ffffff;
            font-weight:bold;
            color:#017f6a;
        }
        QTabBar::tab:selected {
            border-bottom:2px solid #017f6a;
        }
    )");

    mainLayout->addWidget(tabs);

    // --- Messages zone with scroll ---
    messagesScrollArea = new QScrollArea(this);
    messagesScrollArea->setWidgetResizable(true);
    messagesScrollArea->setFrameShape(QFrame::NoFrame);
    messagesScrollArea->setStyleSheet("background-color:#ffffff; border:none;");

    messagesContainer = new QWidget();
    messagesContainer->setStyleSheet("background-color:#ffffff;");

    messagesLayout = new QVBoxLayout(messagesContainer);
    messagesLayout->setContentsMargins(10,10,10,10);
    messagesLayout->setSpacing(8);
    messagesLayout->setAlignment(Qt::AlignTop);
    messagesLayout->addStretch();

    messagesScrollArea->setWidget(messagesContainer);
    mainLayout->addWidget(messagesScrollArea, 1);

    // --- Input zone ---
    QFrame* inputFrame = new QFrame(this);
    inputFrame->setStyleSheet("background-color:#ffffff;");
    inputFrame->setFixedHeight(70);

    QHBoxLayout* inputLayout = new QHBoxLayout(inputFrame);
    inputLayout->setContentsMargins(10,10,10,10);
    inputLayout->setSpacing(0);

    // Container for input and icons
    QFrame* editContainer = new QFrame(this);
    editContainer->setStyleSheet("background:#e8fff7; border-radius:6px;");

    QHBoxLayout* editLayout = new QHBoxLayout(editContainer);
    editLayout->setContentsMargins(10,0,10,0);
    editLayout->setSpacing(5);

    // Input message
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Write a message...");
    messageInput->setStyleSheet("background:transparent; border:none; font-size:14px; color:black;");
    editLayout->addWidget(messageInput, 1);

    // Icon paperclip
    QToolButton* paperclipBtn = new QToolButton(this);
    paperclipBtn->setIcon(QIcon(":/assets/icons/paperclip.png"));
    paperclipBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(paperclipBtn);

    // Icon emoji
    QToolButton* emojiBtn = new QToolButton(this);
    emojiBtn->setIcon(QIcon(":/assets/icons/smiley.png"));
    emojiBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(emojiBtn);

    // Icon image
    QToolButton* imageBtn = new QToolButton(this);
    imageBtn->setIcon(QIcon(":/assets/icons/image.png"));
    imageBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(imageBtn);

    // Separator
    QWidget* separator = new QWidget(this);
    separator->setFixedWidth(1);
    separator->setFixedHeight(30);
    separator->setStyleSheet("background-color:#bdbdbd; margin:0; padding:0;");
    editLayout->addWidget(separator);

    // Button send
    QToolButton* sendBtn = new QToolButton(this);
    sendBtn->setIcon(QIcon(":/assets/icons/sent.png"));
    sendBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(sendBtn);

    connect(sendBtn, &QToolButton::clicked, this, [this, service]() {
        QString text = messageInput->text().trimmed();

        if (!text.isEmpty() && activeChatId != -1) {
            if (conversationType == "personal")
                service->sendPersonal(activeChatId, text.toStdString());
            else if (conversationType == "group")
                service->sendGroup(activeChatId, text.toStdString());

            addMessage(service->userId(), tr("CHAT_PAGE.YOU"), text);

            messageInput->clear();

            QScrollBar* scrollBar = messagesScrollArea->verticalScrollBar();
            scrollBar->setValue(scrollBar->maximum());
        }
    });

    inputLayout->addWidget(editContainer, 1);
    mainLayout->addWidget(inputFrame);
}

void ChatPage::clearMessages()
{
    QLayoutItem* item;
    while ((item = messagesLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    messagesLayout->addStretch();
}

void ChatPage::addMessage(int senderId, const QString& senderName, const QString& content)
{
    QLabel* messageLabel = new QLabel(messagesContainer);
    messageLabel->setWordWrap(true);
    messageLabel->setTextFormat(Qt::RichText);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    if (senderId == service->userId()) {
        messageLabel->setText("<b>" + tr("CHAT_PAGE.YOU") + " :</b> " + content);
        messageLabel->setStyleSheet(R"(
            background-color:#e8fff7;
            color:black;
            border:none;
            border-radius:8px;
            padding:10px;
            font-size:14px;
        )");
    } else {
        messageLabel->setText("<b>" + senderName + " :</b> " + content);
        messageLabel->setStyleSheet(R"(
            background-color:#f5f5f5;
            color:black;
            border:none;
            border-radius:8px;
            padding:10px;
            font-size:14px;
        )");
    }

    int insertIndex = messagesLayout->count() - 1;
    messagesLayout->insertWidget(insertIndex, messageLabel);
}

void ChatPage::loadConversation(int chatId, const QString& conversationName, const QString& type)
{
    activeChatId = chatId;
    conversationType = type.toStdString();
    userNameLabel->setText(conversationName);

    clearMessages();

    QVector<ChatMessage> personalMessages = service->getMessages(chatId, type.toStdString());

    if (personalMessages.isEmpty()) {
        QLabel* emptyLabel = new QLabel(tr("CHAT_PAGE.NO_MESSAGES"), messagesContainer);
        emptyLabel->setStyleSheet(R"(
            background-color:#ffffff;
            color:#808080;
            border:none;
            padding:10px;
            font-size:14px;
        )");
        emptyLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        int insertIndex = messagesLayout->count() - 1;
        messagesLayout->insertWidget(insertIndex, emptyLabel);
    } else {
        for (const auto& msg : personalMessages) {
            addMessage(msg.senderId, msg.senderName, msg.content);
        }
    }

    QScrollBar* scrollBar = messagesScrollArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}