#include "Login.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QCheckBox>

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    // Flag language selector
    LanguageSelector *langSelector = new LanguageSelector(this);
    connect(langSelector, &LanguageSelector::languageChanged, this, [=](const QString &lang) {});

    // Main frame
    QFrame *container = new QFrame(this);
    container->setFrameShape(QFrame::StyledPanel);
    container->setFrameShadow(QFrame::Raised);
    container->setLineWidth(2);
    container->setObjectName("loginContainer");
    container->setFixedWidth(450);
    container->setFixedHeight(500);

    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(20, 20, 20, 20);
    containerLayout->setSpacing(15);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(container);
    shadow->setBlurRadius(20);
    shadow->setXOffset(0);
    shadow->setYOffset(5);
    container->setGraphicsEffect(shadow);
    container->setMaximumWidth(850);

    // Title
    QLabel *title = new QLabel(tr("LOGIN.PAGE_TITLE"), this);
    title->setAlignment(Qt::AlignLeft);
    title->setObjectName("title");
    title->setAlignment(Qt::AlignLeft);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);
    headerLayout->addWidget(title, 0, Qt::AlignLeft);
    headerLayout->addStretch(1);
    headerLayout->addWidget(langSelector, 0, Qt::AlignRight);

    containerLayout->addLayout(headerLayout);

    // Separator
    QLabel *separator = new QLabel(this);
    separator->setFixedHeight(2);
    separator->setObjectName("horizontalSeparator");
    containerLayout->addWidget(separator);

    // Email
    QLabel *emailLabel = new QLabel(tr("LOGIN.EMAIL"), this);
    emailLabel->setObjectName("label");
    QLineEdit *emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText(tr("LOGIN.ENTER_YOUR_EMAIL"));
    emailEdit->setObjectName("input");
    emailEdit->addAction(QIcon(":/assets/icons/envelope.png"), QLineEdit::LeadingPosition);

    QVBoxLayout *emailLayout = new QVBoxLayout();
    emailLayout->setSpacing(0);
    emailLayout->setContentsMargins(0, 0, 0, 0);
    emailLayout->addWidget(emailLabel);
    emailLayout->addWidget(emailEdit);

    containerLayout->addLayout(emailLayout);

    // Password
    QLabel *passwordLabel = new QLabel(tr("LOGIN.PASSWORD"), this);
    passwordLabel->setObjectName("label");
    QLineEdit *passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText(tr("LOGIN.ENTER_YOUR_PASSWORD"));
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setObjectName("input");
    passwordEdit->addAction(QIcon(":/assets/icons/lock.png"), QLineEdit::LeadingPosition);

    QVBoxLayout *passwordLayout = new QVBoxLayout();
    passwordLayout->setSpacing(0);
    passwordLayout->setContentsMargins(0, 0, 0, 0);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);

    containerLayout->addLayout(passwordLayout);

    // Checkbox
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    optionsLayout->setSpacing(0);
    QCheckBox *rememberCheck = new QCheckBox(tr("LOGIN.REMEMBER_ME"), this);
    rememberCheck->setObjectName("checkbox");
    rememberCheck->setCursor(Qt::PointingHandCursor);

    // Lost Password
    QString lostPasswordHtml = QString("<a href='#'>%1</a>").arg(tr("LOGIN.LOST_PASSWORD"));
    QLabel *forgotLabel = new QLabel(lostPasswordHtml, this);
    forgotLabel->setTextFormat(Qt::RichText);
    forgotLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgotLabel->setOpenExternalLinks(false);
    forgotLabel->setAlignment(Qt::AlignRight);

    containerLayout->addLayout(optionsLayout);
    optionsLayout->addWidget(rememberCheck, 0, Qt::AlignVCenter);
    optionsLayout->addStretch(1);
    optionsLayout->addWidget(forgotLabel, 0, Qt::AlignVCenter);

    // Button
    QPushButton *loginButton = new QPushButton(tr("LOGIN.PAGE_TITLE"), this);
    loginButton->setObjectName("primaryButton");
    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    loginButton->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(loginButton);

    // No account and subcribe link
    QString signupText = tr("LOGIN.NO_ACCOUNT") + " " + QString("<a style='color:#00473c; href='#'>%1</a>").arg(tr("LOGIN.SIGN_UP"));
    QLabel *signupLabel = new QLabel(signupText, this);
    signupLabel->setTextFormat(Qt::RichText);
    signupLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    signupLabel->setOpenExternalLinks(false);
    signupLabel->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(signupLabel);

    // TO DO: link subscribe button
    connect(loginButton, &QPushButton::clicked, this, [=]()
    {
        qDebug() << "Login button clicked with email:" << emailEdit->text() << "and password:" << passwordEdit->text();
        QString email = emailEdit->text();
        QString password = passwordEdit->text();
        emit loginRequested(emailEdit->text(), passwordEdit->text());
        // TO DO: guest also language
    });

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(container, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
}
