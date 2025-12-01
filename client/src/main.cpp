#include "ClientApp.h"
#include "MainWindow.h"
#include "TranslationManager.h"
#include "MessagingClient.h"

#include <QApplication>
#include <QMetaObject>
#include <QDebug>
#include <drogon/drogon.h>
#include <trantor/net/EventLoop.h>
#include <future>
#include <thread>

// TO DO: remove after debug
#include <iostream>

static MessagingClient* g_wsClient = nullptr;
static trantor::EventLoop* g_loop = nullptr;

/* ===========================
 * Qt Window + event loop
 * =========================== */
int runQtWindow(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TranslationManager translationManager(&app);
    QString osLang = translationManager.getOsLanguage();
    translationManager.loadLanguage(osLang);

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;

    MainWindow mainWindow;
    mainWindow.show();

    int ret = app.exec();

    return ret;
}

/* ===========================
 * Drogon / Trantor backend
 * =========================== */
void startBackend()
{
    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();

    std::thread loopThread([&loopProm]()
    {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    g_loop = loopFut.get();

    auto httpClient =
        drogon::HttpClient::newHttpClient("http://127.0.0.1:8080", g_loop);

    ClientApp tmpClient(httpClient);
    std::string fakeUser = tmpClient.selectFakeUser();

    qDebug() << "Fake user selected:"
             << QString::fromStdString(fakeUser);

    // Init WS client in Qt thread
    QMetaObject::invokeMethod(
        g_wsClient,
        [fakeUser]()
        {
            g_wsClient->setUserId(QString::fromStdString(fakeUser));
            g_wsClient->maybeConnect(true);
        },
        Qt::QueuedConnection
    );

    auto wsSendCb = [](const std::string& msg)
    {
        if (!g_wsClient) return;

        const QString qmsg = QString::fromStdString(msg);
        QMetaObject::invokeMethod(
            g_wsClient,
            [qmsg]()
            {
                g_wsClient->sendTestMessage(qmsg);
            },
            Qt::QueuedConnection
        );
    };

    ClientApp drogonClient(httpClient, wsSendCb);
    drogonClient.run();

    // Quit Qt when backend stops
    QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);

    g_loop->queueInLoop([] { g_loop->quit(); });
    loopThread.join();
}

/* ===========================
 * main
 * =========================== */
int main(int argc, char *argv[])
{
    // WS client must be created in Qt thread
    std::thread backendThread(startBackend);

    int qtRet = runQtWindow(argc, argv);

    backendThread.join();
    return qtRet;
}
