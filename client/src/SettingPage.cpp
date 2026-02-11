#include "SettingPage.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QVBoxLayout>

SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet("background-color: white;");

    // --- Header ---
    QLabel *title = new QLabel(tr("PROFILE_EDIT.TITLE"), this);
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);
    title->move(20, 20);
    title->resize(400, 30);

    QLabel *separator = new QLabel(this);
    separator->setFixedHeight(2);
    separator->setStyleSheet("background-color: #ccc;");
    separator->move(20, 60);
    separator->resize(400, 2);

    int y = 80;

    // First Name
    QLabel *firstNameLabel = new QLabel(tr("PROFILE_EDIT.FIRST_NAME"), this);
    firstNameLabel->move(20, y);
    firstNameLabel->resize(200, 20);
    firstNameLabel->setObjectName("label");

    QLineEdit *firstNameEdit = new QLineEdit(this);
    firstNameEdit->setPlaceholderText(tr("PROFILE_EDIT.ENTER_FIRST_NAME"));
    firstNameEdit->move(20, y + 25);
    firstNameEdit->resize(400, 30);
    firstNameEdit->setObjectName("input");

    y += 65;

    // Last Name
    QLabel *lastNameLabel = new QLabel(tr("PROFILE_EDIT.LAST_NAME"), this);
    lastNameLabel->move(20, y);
    lastNameLabel->resize(200, 20);

    QLineEdit *lastNameEdit = new QLineEdit(this);
    lastNameEdit->setPlaceholderText(tr("PROFILE_EDIT.ENTER_LAST_NAME"));
    lastNameEdit->move(20, y + 5);
    lastNameEdit->resize(400, 30);
    lastNameEdit->setObjectName("input");

    y += 65;

    // Date of Birth
    QLabel *dobLabel = new QLabel(tr("PROFILE_EDIT.DATE_OF_BIRTH"), this);
    dobLabel->move(20, y);
    dobLabel->resize(200, 20);

    QDateEdit *dobEdit = new QDateEdit(this);
    dobEdit->setCalendarPopup(true);
    dobEdit->setDisplayFormat("dd/MM/yyyy");
    dobEdit->setMaximumDate(QDate::currentDate());
    dobEdit->setMinimumDate(QDate(1900, 1, 1));
    dobEdit->move(20, y + 25);
    dobEdit->resize(200, 30);

    y += 65;

    // Email
    QLabel *emailLabel = new QLabel(tr("PROFILE_EDIT.EMAIL"), this);
    emailLabel->move(20, y);
    emailLabel->resize(200, 20);

    QLineEdit *emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText(tr("PROFILE_EDIT.ENTER_EMAIL"));
    emailEdit->move(20, y + 25);
    emailEdit->resize(400, 30);
    emailEdit->setObjectName("input");

    y += 65;

    // Adresse
    QLabel *addressLabel = new QLabel(tr("PROFILE_EDIT.ADDRESS"), this);
    addressLabel->move(20, y);
    addressLabel->resize(200, 20);

    QLineEdit *addressEdit = new QLineEdit(this);
    addressEdit->setPlaceholderText(tr("PROFILE_EDIT.ENTER_ADDRESS"));
    addressEdit->move(20, y + 25);
    addressEdit->resize(400, 30);
    addressEdit->setObjectName("input");

    y += 65;

    // Buttons
    QPushButton *saveButton = new QPushButton(tr("PROFILE_EDIT.SAVE"), this);
    saveButton->move(20, y);
    saveButton->resize(120, 30);

    QPushButton *cancelButton = new QPushButton(tr("PROFILE_EDIT.CANCEL"), this);
    cancelButton->move(160, y);
    cancelButton->resize(120, 30);
}
