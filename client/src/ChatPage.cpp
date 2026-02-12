#include "ChatPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QTabBar>
#include <QFrame>
#include <QToolButton>

ChatPage::ChatPage(ClientService* service, QWidget *parent)
    : QWidget(parent)
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
    QLabel* userName = new QLabel("Jean Dupont", header);
    userName->setStyleSheet("font-size:18px;font-weight:bold; color:black;");
    headerLayout->addWidget(userName);
    headerLayout->addStretch();

    mainLayout->addWidget(header);

    // --- Tabs ---
    QTabBar* tabs = new QTabBar(this);
    tabs->addTab("Conversation");
    tabs->addTab("Fichiers");
    tabs->addTab("Photos");
    tabs->setStyleSheet(R"(
        QTabBar { background:#1e1e1e; }
        QTabBar::tab {
            background:#e8fff7;
            padding:10px;
            border: 2px solid #ffffff;
            font-weight:bold;
        }
        QTabBar::tab:selected {
            border-bottom:2px solid #017f6a;
        }
    )");

    mainLayout->addWidget(tabs);

    QVector<QString> messages;
    QVector<QPair <int, QString>> personalMessages = service->getMessages(2, "personal");
    for (const auto& [senderId, content] : personalMessages) {
        chatView = new QTextEdit(this);
        chatView->setReadOnly(true);
        chatView->setStyleSheet(R"(
            background-color:#ffffff;
            border:none;
            padding:10px;
            font-size:14px;
        )");
        if (senderId == service->userId())
            chatView->append("<b>Vous :</b> " + content);
        else
            chatView->append("<b>Other :</b> " + content);

        mainLayout->addWidget(chatView, 1);
    }
    // channelLayout->addStretch();
    // --- Chat view ---
    // chatView = new QTextEdit(this);
    // chatView->setReadOnly(true);
    // chatView->setStyleSheet(R"(
    //     background-color:#ffffff;
    //     border:none;
    //     padding:10px;
    //     font-size:14px;
    // )");
    // chatView->setText(
    //     "<b>Jean :</b> Salut !<br>"
    //     "<b>Vous :</b> Bonjour, comment ça va ?<br>"
    // );

    // mainLayout->addWidget(chatView, 1);

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
    messageInput->setStyleSheet("background:transparent; border:none; font-size:14px;");
    editLayout->addWidget(messageInput, 1);

    // Icon paperclip, emoji and  image
    QToolButton* paperclipBtn = new QToolButton(this);
    paperclipBtn->setIcon(QIcon(":/assets/icons/paperclip.png"));
    paperclipBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(paperclipBtn);

    QToolButton* emojiBtn = new QToolButton(this);
    emojiBtn->setIcon(QIcon(":/assets/icons/smiley.png"));
    emojiBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(emojiBtn);

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

    // Button sent
    QToolButton* sendBtn = new QToolButton(this);
    sendBtn->setIcon(QIcon(":/assets/icons/sent.png"));
    sendBtn->setStyleSheet("border:none; padding:4px;");
    editLayout->addWidget(sendBtn);

    connect(sendBtn, &QToolButton::clicked, this, [this](){
        QString text = messageInput->text();
        if(!text.isEmpty()) {
            chatView->append("<b>Vous :</b> " + text);
            messageInput->clear();
        }
    });

    inputLayout->addWidget(editContainer, 1);
    mainLayout->addWidget(inputFrame);
}
