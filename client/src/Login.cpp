#include "Login.h"

#include <QCheckBox>
#include <QDateEdit>
#include <QDebug>
#include <QFile>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

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

    // Header with title and language selector
    QLabel *title = new QLabel(tr("LOGIN.PAGE_TITLE"), this);
    title->setObjectName("title");

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addWidget(langSelector);

    containerLayout->addLayout(headerLayout);

    QLabel *separator = new QLabel(this);
    separator->setFixedHeight(2);
    separator->setObjectName("horizontalSeparator");
    containerLayout->addWidget(separator);

    // StackedWidget to switch between login and register
    QStackedWidget *stack = new QStackedWidget(container);
    containerLayout->addWidget(stack);

    // --- Login page ---
    QWidget *loginPage = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(loginPage);

    QLabel *emailLabel = new QLabel(tr("LOGIN.EMAIL"));
    emailLabel->setObjectName("label");

    QLineEdit *emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText(tr("LOGIN.ENTER_YOUR_EMAIL"));
    emailEdit->setObjectName("input");
    emailEdit->addAction(QIcon(":/assets/icons/envelope.png"), QLineEdit::LeadingPosition);

    QLabel *passwordLabel = new QLabel(tr("LOGIN.PASSWORD"));
    passwordLabel->setObjectName("label");
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText(tr("LOGIN.ENTER_YOUR_PASSWORD"));
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setObjectName("input");
    passwordEdit->addAction(QIcon(":/assets/icons/lock.png"), QLineEdit::LeadingPosition);

    QCheckBox *rememberCheck = new QCheckBox(tr("LOGIN.REMEMBER_ME"));
    rememberCheck->setObjectName("checkbox");

    QString lostPasswordHtml = QString("<a href='#'>%1</a>").arg(tr("LOGIN.LOST_PASSWORD"));

    QLabel *forgotLabel = new QLabel(lostPasswordHtml);
    forgotLabel->setTextFormat(Qt::RichText);
    forgotLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QPushButton *loginButton = new QPushButton(tr("LOGIN.PAGE_TITLE"));
    loginButton->setObjectName("primaryButton");
    loginButton->setCursor(Qt::PointingHandCursor);


    QString signupText = tr("LOGIN.NO_ACCOUNT") + " " + QString("<a href='#'>%1</a>").arg(tr("LOGIN.SIGN_UP"));

    QLabel *signupLabel = new QLabel(signupText);
    signupLabel->setTextFormat(Qt::RichText);
    signupLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    signupLabel->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(signupLabel);

    // Layout login
    loginLayout->addWidget(emailLabel);
    loginLayout->addWidget(emailEdit);
    loginLayout->addWidget(passwordLabel);
    loginLayout->addWidget(passwordEdit);
    loginLayout->addWidget(rememberCheck);
    loginLayout->addWidget(forgotLabel);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(signupLabel);
    loginLayout->addStretch();

    stack->addWidget(loginPage);

    // --- Register Page ---
    QWidget *registerPage = new QWidget();
    QVBoxLayout *registerLayout = new QVBoxLayout(registerPage);

    // First name + Last name
    QHBoxLayout *nameLayout = new QHBoxLayout();

    QVBoxLayout *firstNameLayout = new QVBoxLayout();
    QLabel *regFirstNameLabel = new QLabel(tr("REGISTER.FIRST_NAME_LABEL"));
    regFirstNameLabel->setObjectName("label");

    QLineEdit *regFirstNameEdit = new QLineEdit();
    regFirstNameEdit->setPlaceholderText(tr("REGISTER.ENTER_YOUR_FIRST_NAME"));
    regFirstNameEdit->setObjectName("input");
    regFirstNameEdit->addAction(QIcon(":/assets/icons/person.png"), QLineEdit::LeadingPosition);

    firstNameLayout->addWidget(regFirstNameLabel);
    firstNameLayout->addWidget(regFirstNameEdit);

    QVBoxLayout *lastNameLayout = new QVBoxLayout();
    QLabel *regLastNameLabel = new QLabel(tr("REGISTER.LAST_NAME_LABEL"));
    regLastNameLabel->setObjectName("label");

    QLineEdit *regLastNameEdit = new QLineEdit();
    regLastNameEdit->setPlaceholderText(tr("REGISTER.ENTER_YOUR_LAST_NAME"));
    regLastNameEdit->setObjectName("input");
    regLastNameEdit->addAction(QIcon(":/assets/icons/person.png"), QLineEdit::LeadingPosition);

    lastNameLayout->addWidget(regLastNameLabel);
    lastNameLayout->addWidget(regLastNameEdit);

    nameLayout->addLayout(firstNameLayout);
    nameLayout->addLayout(lastNameLayout);

    // Date of Birth
    QLabel *dobLabel = new QLabel(tr("REGISTER.DATE_OF_BIRTH"));
    dobLabel->setObjectName("label");

    QDateEdit *dobEdit = new QDateEdit();
    dobEdit->setCalendarPopup(true);
    dobEdit->setDisplayFormat("dd/MM/yyyy");
    dobEdit->setMaximumDate(QDate::currentDate());
    dobEdit->setMinimumDate(QDate(1900, 1, 1));
    dobEdit->setObjectName("dobEdit");

    dobEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    dobEdit->setSpecialValueText(tr("REGISTER.SELECT_YOUR_DOB"));
    dobEdit->setDate(dobEdit->minimumDate());
    dobEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    dobEdit->setFixedHeight(36);

    // Email
    QLabel *regEmailLabel = new QLabel(tr("REGISTER.EMAIL_LABEL"));
    regEmailLabel->setObjectName("label");

    QLineEdit *regEmailEdit = new QLineEdit();
    regEmailEdit->setPlaceholderText(tr("REGISTER.ENTER_YOUR_EMAIL"));
    regEmailEdit->setObjectName("input");
    regEmailEdit->addAction(QIcon(":/assets/icons/envelope.png"), QLineEdit::LeadingPosition);

    // Password + Confirm password
    QHBoxLayout *passwordLayout = new QHBoxLayout();

    QVBoxLayout *passLayout = new QVBoxLayout();
    QLabel *regPasswordLabel = new QLabel(tr("REGISTER.PASSWORD_LABEL"));
    regPasswordLabel->setObjectName("label");

    QLineEdit *regPasswordEdit = new QLineEdit();
    regPasswordEdit->setPlaceholderText(tr("REGISTER.ENTER_YOUR_PASSWORD"));
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setObjectName("input");
    regPasswordEdit->addAction(QIcon(":/assets/icons/lock.png"), QLineEdit::LeadingPosition);

    passLayout->addWidget(regPasswordLabel);
    passLayout->addWidget(regPasswordEdit);

    QVBoxLayout *confirmLayout = new QVBoxLayout();
    QLabel *regConfirmPasswordLabel = new QLabel(tr("REGISTER.CONFIRM_PASSWORD_LABEL"));
    regConfirmPasswordLabel->setObjectName("label");

    QLineEdit *regConfirmPasswordEdit = new QLineEdit();
    regConfirmPasswordEdit->setPlaceholderText( tr("REGISTER.ENTER_YOUR_CONFIRM_PASSWORD"));
    regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    regConfirmPasswordEdit->setObjectName("input");
    regConfirmPasswordEdit->addAction(QIcon(":/assets/icons/lock.png"), QLineEdit::LeadingPosition);

    confirmLayout->addWidget(regConfirmPasswordLabel);
    confirmLayout->addWidget(regConfirmPasswordEdit);

    passwordLayout->addLayout(passLayout);
    passwordLayout->addLayout(confirmLayout);

    // Create account button
    QPushButton *createAccount = new QPushButton(tr("LOGIN.SIGN_UP"));
    createAccount->setObjectName("primaryButton");

    // Back to login link
    QLabel *backLogin = new QLabel("<a href='#'>Back to login</a>");
    backLogin->setTextFormat(Qt::RichText);
    backLogin->setTextInteractionFlags(Qt::TextBrowserInteraction);
    backLogin->setAlignment(Qt::AlignCenter);

    // Main layout register
    registerLayout->addLayout(nameLayout);
    registerLayout->addWidget(dobLabel);
    registerLayout->addWidget(dobEdit);
    registerLayout->addWidget(regEmailLabel);
    registerLayout->addWidget(regEmailEdit);
    registerLayout->addLayout(passwordLayout);
    registerLayout->addWidget(createAccount);
    registerLayout->addWidget(backLogin);
    registerLayout->addStretch();

    stack->addWidget(registerPage);

    // Connections
    connect(signupLabel, &QLabel::linkActivated, this, [=](const QString &) {
        stack->setCurrentWidget(registerPage);
        title->setText(tr("LOGIN.SIGN_UP"));
    });

    connect(backLogin, &QLabel::linkActivated, this, [=](const QString &) {
        stack->setCurrentWidget(loginPage);
        title->setText(tr("LOGIN.PAGE_TITLE"));
    });

    connect(loginButton, &QPushButton::clicked, this, [=]()
    {
        emit loginRequested(emailEdit->text(), passwordEdit->text());
        // TO DO: guest also language
    });

    connect(createAccount, &QPushButton::clicked, this, [=]()
    {
        if (regPasswordEdit->text() != regConfirmPasswordEdit->text())
        {
            qDebug() << "Passwords do not match!";
            //TODO show error message to user
            return;
        }
        emit registerRequested(regEmailEdit->text(), regPasswordEdit->text(), regFirstNameEdit->text(), regLastNameEdit->text(), dobEdit->text());
    });

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(container, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
}
