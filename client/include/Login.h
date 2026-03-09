#ifndef LOGIN_H
#define LOGIN_H

#include "ChatPage.h"
#include "LanguageSelector.h"
#include "TranslationManager.h"

#include <QWidget>

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr, TranslationManager *tm = nullptr);

signals:
    /**
     * Signal emitted when the user requests to log in, with the provided email and password.
     * @param email The email entered by the user.
     * @param password The password entered by the user.
     */
    void loginRequested(
        const QString &email,
        const QString &password);

    /**
     * Signal emitted when the user requests to register a new account, with the provided details.
     * @param email The email entered by the user.
     * @param password The password entered by the user.
     * @param firstName The first name entered by the user.
     * @param lastName The last name entered by the user.
     * @param dob The date of birth entered by the user.
     */
    void registerRequested(
        const QString &email,
        const QString &password,
        const QString &firstName,
        const QString &lastName,
        const QString &dob);

    /**
     * Signal emitted when the language is changed in the language selector, with the new language code.
     * @param lang The new language code selected by the user.
     */
    void languageChanged(const QString& lang);

private:
    TranslationManager *translationManager_;
};

#endif // LOGIN_H
