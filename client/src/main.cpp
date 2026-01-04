#include <QApplication>
#include "MainWindow.h"
#include "TranslationManager.h"

#include "ClientApp.h"
#include <drogon/drogon.h>
#include <trantor/net/EventLoop.h>
#include <thread>
#include <future>

// TO DO: remove after debug
#include <iostream>
using namespace std;

int runQtWindow(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TranslationManager translationManager(&app);
    QString osLang = translationManager.getOsLanguage();
    translationManager.loadLanguage(osLang);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

int main(int argc, char *argv[])
{
    // Drogon / Trantor
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
    // drogonClient.run();

    // Qt Window
    int qtRet = runQtWindow(argc, argv);

    // Stop Drogon loop
    loop->queueInLoop([loop]{ loop->quit(); });
    loopThread.join();

    return qtRet;
}
