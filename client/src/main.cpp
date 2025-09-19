#include <QApplication>
#include <QDebug>
#include <iostream>

#include "../../common/include/TranslationManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TranslationManager translationManager(&app);

    translationManager.initialize();

    cout << QObject::tr("K.Hello").toStdString() << endl;

    return app.exec();
}
#include <drogon/drogon.h>
#include "ClientApp.h"
#include <trantor/net/EventLoop.h>
#include <thread>
#include <future>

int main() {
    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();
    std::thread loopThread([&loopProm]() {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    trantor::EventLoop* loop = loopFut.get();
    auto client = drogon::HttpClient::newHttpClient("http://127.0.0.1:8080", loop);

    ClientApp app(client);
    app.run();

    // Stop loop proprement
    loop->queueInLoop([loop]{ loop->quit(); });
    loopThread.join();
    return 0;
}
