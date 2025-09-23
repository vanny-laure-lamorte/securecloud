#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include <drogon/drogon.h>
#include <trantor/net/EventLoop.h>

#include "ClientApp.h"
#include "../../common/include/TranslationManager.h"

#include <thread>
#include <future>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TranslationManager translationManager(&app);
    translationManager.initialize();

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;

    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();

    std::thread loopThread([&loopProm]() {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    trantor::EventLoop* loop = loopFut.get();
    auto client = drogon::HttpClient::newHttpClient("http://127.0.0.1:8080", loop);

    ClientApp drogonClient(client);

    drogonClient.run();

    loop->queueInLoop([loop]{ loop->quit(); });
    loopThread.join();

    return 0;
}
