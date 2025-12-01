#include <QCoreApplication>
#include <QDebug>
#include <drogon/drogon.h>
#include <trantor/net/EventLoop.h>
#include <iostream>
#include <thread>
#include <future>

#include "ClientApp.h"
#include "TranslationManager.h"
#include "MessagingClient.h"
#include <QMetaObject>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TranslationManager translationManager(&app);
    translationManager.initialize();

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;

    // Event loop Drogon (HTTP)
    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();

    std::thread loopThread([&loopProm]() {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    trantor::EventLoop* loop = loopFut.get();
    auto httpClient = drogon::HttpClient::newHttpClient("http://127.0.0.1:8080", loop);

    ClientApp tmpClient(httpClient);
    std::string fakeUser = tmpClient.selectFakeUser();
    qDebug() << "Fake user selected:" << QString::fromStdString(fakeUser);

    MessagingClient wsClient(QString::fromStdString(fakeUser));
    bool auth = true;
    wsClient.maybeConnect(auth);

    auto wsSendCb = [&wsClient](const std::string &msg) {
        QString qmsg = QString::fromStdString(msg);
        QMetaObject::invokeMethod(
            &wsClient,
            [qmsg, &wsClient]() {
                wsClient.sendTestMessage(qmsg);
            },
            Qt::QueuedConnection
        );
    };

    ClientApp drogonClient(httpClient, wsSendCb);

    std::thread consoleThread([&]() {
        drogonClient.run();
        QMetaObject::invokeMethod(&app, "quit", Qt::QueuedConnection);
    });

    int ret = app.exec();

    loop->queueInLoop([loop]{ loop->quit(); });
    loopThread.join();
    consoleThread.join();

    return ret;
}
