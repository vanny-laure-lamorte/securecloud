#include "ClientApp.h"
#include "MainWindow.h"
#include "TranslationManager.h"
#include "api/AuditClient.h"
#include "api/AuthClient.h"
#include "core/ClientState.h"
#include "core/HttpGatewayClient.h"
#include "core/MessagingClient.h"
#include "core/WsGatewayClient.h"

#include <drogon/drogon.h>
#include <future>
#include <iostream>
#include <thread>
#include <trantor/net/EventLoop.h>
#include <QApplication>
#include <QMetaObject>
#include <QDebug>

static MessagingClient* g_wsClient = nullptr;
static trantor::EventLoop* g_loop = nullptr;

static std::promise<void> g_qtReadyProm;
static std::shared_future<void> g_qtReadyFut;

int runQtWindow(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TranslationManager translationManager(&app);
    const QString osLang = translationManager.getOsLanguage();
    translationManager.loadLanguage(osLang);

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;

    MessagingClient wsClient(QStringLiteral("TEMP_USER"));
    g_wsClient = &wsClient;

    g_qtReadyProm.set_value();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

void startBackend()
{
    g_qtReadyFut.wait();

    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();

    std::thread loopThread([&loopProm]()
    {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    g_loop = loopFut.get();

    auto httpClient = drogon::HttpClient::newHttpClient("http://127.0.0.1:8080", g_loop);
    auto clientState = std::make_shared<ClientState>();
    HttpGatewayClient httpGateway(httpClient, clientState);
    AuthClient authClient(httpGateway, clientState);
    AuditClient auditClient(httpGateway);
    WsGatewayClient wsGateway{ g_wsClient };
    ClientApp consoleApp(authClient, auditClient, &wsGateway);
    consoleApp.run();

    const std::string jwtStd = clientState->jwt;
    const QString jwt = QString::fromStdString(jwtStd);
    QMetaObject::invokeMethod(
        g_wsClient,
        [jwt, clientState]()
        {
            if (!g_wsClient) return;
            g_wsClient->setUserId(QString::fromStdString(clientState->userName));
            g_wsClient->maybeConnect(jwt);
        },
        Qt::QueuedConnection
    );

    QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);

    g_loop->queueInLoop([] { g_loop->quit(); });
    loopThread.join();
}

int main(int argc, char *argv[])
{
    g_qtReadyFut = g_qtReadyProm.get_future().share();

    std::thread backendThread(startBackend);

    int qtRet = runQtWindow(argc, argv);

    backendThread.join();
    return qtRet;
}
