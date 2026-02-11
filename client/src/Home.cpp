#include "Home.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QIcon>
#include <QScrollArea>
#include <QLineEdit>
#include <QStackedWidget>
#include <QHBoxLayout>

Home::Home(QWidget *parent)
    : QWidget(parent)
{
    // --- Layout principal horizontal ---
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- Left Sidebar ---
    QFrame *leftSidebar = new QFrame(this);
    leftSidebar->setFixedWidth(70);
    leftSidebar->setObjectName("leftSidebar");
    leftSidebar->setStyleSheet("#leftSidebar { background-color: #002d24; }");
    leftSidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QVBoxLayout *leftLayout = new QVBoxLayout(leftSidebar);
    leftLayout->setContentsMargins(0, 10, 0, 10);
    leftLayout->setSpacing(10);

    // Avatar + status dot
    QFrame *avatarContainer = new QFrame(leftSidebar);
    avatarContainer->setFixedSize(42, 42);
    leftLayout->addWidget(avatarContainer, 0, Qt::AlignHCenter);

    QLabel *avatarLabel = new QLabel(avatarContainer);
    QPixmap avatar(":/assets/icons/user-profil.png");
    avatarLabel->setPixmap(avatar.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    avatarLabel->setFixedSize(40, 40);
    avatarLabel->move(1, 1);
    avatarLabel->setStyleSheet("border-radius:20px");

    QLabel *statusDot = new QLabel(avatarContainer);
    statusDot->setFixedSize(10, 10);
    statusDot->move(30, 30);
    statusDot->setObjectName("statusDot");
    statusDot->raise();

    // Separator under logo
    QFrame *logoSeparator = new QFrame(leftSidebar);
    logoSeparator->setFrameShape(QFrame::HLine);
    logoSeparator->setFrameShadow(QFrame::Sunken);
    logoSeparator->setFixedSize(40, 1);
    logoSeparator->setObjectName("separator");
    leftLayout->addWidget(logoSeparator, 0, Qt::AlignHCenter);

    QStackedWidget *stackedContent = new QStackedWidget();
    stackedContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    stackedContent->setStyleSheet("background-color: #f6f8fc");

    // Stacked widget pages
    ChatPage *chatPage = new ChatPage();
    GroupPage *groupPage = new GroupPage();
    ContactPage *contactPage = new ContactPage();
    CalendarPage *calendarPage = new CalendarPage();
    ActivityPage *activityPage = new ActivityPage();
    FilterPage *filterPage = new FilterPage();
    EmergencyPage *emergencyPage = new EmergencyPage();
    SettingPage *settingPage = new SettingPage();

    /*
    *  * 2. Group Page
    * 3. Contact Page
    * 4. Calendar Page
    * 5. Activity Page
    * 6. Filter Page
    * 8. Emergency Page
    * 7. Setting Page
    */
    stackedContent->addWidget(chatPage);
    stackedContent->addWidget(groupPage);
    stackedContent->addWidget(contactPage);
    stackedContent->addWidget(calendarPage);
    stackedContent->addWidget(activityPage);
    stackedContent->addWidget(filterPage);
    stackedContent->addWidget(emergencyPage);
    stackedContent->addWidget(settingPage);

    // left Sidebar Buttons: Chat, Group, Contact, Calendar, Activity
    struct SidebarButton
    {
        QString text;
        QString iconPath;
    };

    QVector<SidebarButton> buttons = {
        {tr("MAIN_PAGE.CHAT"), ":/assets/icons/chat.png"},
        {tr("MAIN_PAGE.GROUP"), ":/assets/icons/group.png"},
        {tr("MAIN_PAGE.CONTACT"), ":/assets/icons/contacts.png"},
        {tr("MAIN_PAGE.CALENDAR"), ":/assets/icons/calendar.png"},
        {tr("MAIN_PAGE.ACTIVITY"), ":/assets/icons/notification.png"}};

    for (int i = 0; i < buttons.size(); ++i)
    {
        QPushButton *btn = new QPushButton(leftSidebar);
        btn->setIcon(QIcon(buttons[i].iconPath));
        btn->setIconSize(QSize(24, 24));
        btn->setFixedSize(50, 50);
        btn->setToolTip(buttons[i].text);
        btn->setStyleSheet(
            "QPushButton { border-radius: 25px; background-color: #00473c;}"
            "QPushButton:hover { background-color: #017f6a;}");
        leftLayout->addWidget(btn, 0, Qt::AlignHCenter);

        // Connexion lambda avec l'indice correct
        connect(btn, &QPushButton::clicked, [stackedContent, i]()
            { stackedContent->setCurrentIndex(i);
        });
    }
    leftLayout->addStretch();

    // Button Danger
    QPushButton *emergencyBtn = new QPushButton(leftSidebar);
    emergencyBtn->setIcon(QIcon(":/assets/icons/danger.png"));
    emergencyBtn->setIconSize(QSize(24, 24));
    emergencyBtn->setFixedSize(50, 50);
    emergencyBtn->setToolTip(tr("MAIN_PAGE.DANGER"));
    emergencyBtn->setObjectName("emergencyBtn");

    leftLayout->addWidget(emergencyBtn, 0, Qt::AlignHCenter);
        connect(emergencyBtn, &QPushButton::clicked, [stackedContent]() {
        stackedContent->setCurrentIndex(6);
    });

    // Button Settings
    QPushButton *settingsBtn = new QPushButton(leftSidebar);
    settingsBtn->setIcon(QIcon(":/assets/icons/settings.png"));
    settingsBtn->setIconSize(QSize(24, 24));
    settingsBtn->setFixedSize(50, 50);
    settingsBtn->setToolTip(tr("MAIN_PAGE.SETTINGS"));
    settingsBtn->setObjectName("NoBackgroundBtn");

    leftLayout->addWidget(settingsBtn, 0, Qt::AlignHCenter);
    leftLayout->addWidget(settingsBtn, 0, Qt::AlignHCenter);

    connect(settingsBtn, &QPushButton::clicked, [stackedContent]()
        { stackedContent->setCurrentIndex(7);
    });

    // --- Separator between left and right sidebar ---
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setFixedWidth(1);
    separator->setStyleSheet("background-color: #404142;");
    separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // --- Right Sidebar dynamique ---
    QFrame *rightArea = new QFrame(this);
    rightArea->setStyleSheet("background-color:#002d24;");
    rightArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *rightAreaVLayout = new QVBoxLayout(rightArea);
    rightAreaVLayout->setContentsMargins(0, 0, 0, 0);
    rightAreaVLayout->setSpacing(0);

    // profile header
    QFrame *profileHeader = new QFrame(rightArea);
    profileHeader->setFixedHeight(70);
    profileHeader->setStyleSheet("background-color:#002d24;");
    profileHeader->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *profileLayout = new QHBoxLayout(profileHeader);
    profileLayout->setContentsMargins(10, 5, 10, 5);
    profileLayout->setSpacing(5);

    // User info
    QVBoxLayout *userInfoLayout = new QVBoxLayout();
    userInfoLayout->setSpacing(2);
    userInfoLayout->setContentsMargins(0, 0, 0, 0);
    userInfoLayout->setSpacing(0);

    // User name
    QLabel *userName = new QLabel("John Doe");
    userName->setStyleSheet("color:white; font-weight:bold; font-size:12px; line-height:12px;");
    userName->setContentsMargins(0, 0, 0, 0);
    userName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    userName->setFixedHeight(10);
    userInfoLayout->addWidget(userName);

    // User role
    QLabel *userRole = new QLabel(tr("MAIN_PAGE.USER_ROLE"));
    userRole->setStyleSheet("color:#cccccc; font-size:10px;");
    userRole->setContentsMargins(0, 0, 0, 0);
    userRole->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    userRole->setFixedHeight(12);
    userInfoLayout->addWidget(userRole);

    // Layout user icônes
    QHBoxLayout *iconsLayout = new QHBoxLayout();
    iconsLayout->setSpacing(10);
    iconsLayout->setContentsMargins(0, 0, 0, 0);

    // Search bar and icon search
    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText(tr("MAIN_PAGE.SEARCH_PLACEHOLDER"));
    searchBar->setFixedHeight(30);
    searchBar->setObjectName("searchBar");

    QPixmap pix(":/assets/icons/search.png");
    QPixmap scaled = pix.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QAction *searchAction = new QAction(QIcon(scaled), "", searchBar);
    searchBar->addAction(searchAction, QLineEdit::TrailingPosition);

    // User options: filter, camera, new chat
    struct UserOptionsBtn
    {
        QString text;
        QString iconPath;
    };

    QVector<UserOptionsBtn> userOptionsBtns = {
        {tr("MAIN_PAGE.FILTER"), ":/assets/icons/filter.png"},
        {tr("MAIN_PAGE.CAMERA"), ":/assets/icons/camera.png"},
        {tr("MAIN_PAGE.NEW_CHAT"), ":/assets/icons/new-chat.png"},
    };

    for (const UserOptionsBtn &btnData : userOptionsBtns)
    {
        QPushButton *btn = new QPushButton(profileHeader);
        btn->setIcon(QIcon(btnData.iconPath));
        btn->setIconSize(QSize(16, 16));
        btn->setFixedSize(30, 30);
        btn->setToolTip(btnData.text);
        btn->setStyleSheet(
            "QPushButton { border-radius: 15px; background-color: #00473c; }"
            "QPushButton:hover { background-color: #017f6a; }"
        );
        iconsLayout->addWidget(btn);
    }

    profileLayout->addLayout(userInfoLayout);
    profileLayout->addSpacing(30);
    profileLayout->addLayout(iconsLayout);
    profileLayout->addStretch();
    profileLayout->addWidget(searchBar);

    rightAreaVLayout->addWidget(profileHeader);

    // Channels + dynamic main content
    QFrame *contentArea = new QFrame(rightArea);
    contentArea->setStyleSheet("background-color:#002d24;");
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *contentLayout = new QHBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // --- Channels panel scrollable ---
    QFrame *channelSidebar = new QFrame(contentArea);
    channelSidebar->setFixedWidth(260);
    channelSidebar->setStyleSheet("background-color:#002d24;");

    QVBoxLayout *channelLayout = new QVBoxLayout(channelSidebar);
    channelLayout->setContentsMargins(0, 0, 0, 0);
    channelLayout->setSpacing(0);

    QLabel *channelTitle = new QLabel("Channels", channelSidebar);
    channelTitle->setStyleSheet("color:#B9BBBE;font-weight:bold;");
    channelLayout->addWidget(channelTitle);

    QVector<QString> channels = {"#general", "#chat1", "#chat2", "#groupe1", "#groupe2", "#random", "#development", "#design", "#marketing", "#sales", "#support", "#hr"};

    for (const QString &ch : channels)
    {
        QLabel *lbl = new QLabel(ch, channelSidebar);
        lbl->setObjectName("lbl");
        channelLayout->addWidget(lbl);
    }
    channelLayout->addStretch();

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(channelSidebar);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFixedWidth(260);
    scrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    contentLayout->addWidget(scrollArea);

    // --- Main section dynamic ---
    QFrame *mainContent = new QFrame(contentArea);
    mainContent->setStyleSheet("background-color:#1e1f22;");
    mainContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentLayout->addWidget(mainContent);

    // Add stacked widget to main content
    QVBoxLayout *mainContentLayout = new QVBoxLayout(mainContent);
    mainContentLayout->setContentsMargins(0, 0, 0, 0);
    mainContentLayout->addWidget(stackedContent);

    contentLayout->addWidget(mainContent);

    rightAreaVLayout->addWidget(contentArea);

    // Assemble main layout
    mainLayout->addWidget(leftSidebar);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(rightArea);
}
