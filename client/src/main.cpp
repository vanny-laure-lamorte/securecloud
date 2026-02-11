#include "MainWindow.h"
#include "TranslationManager.h"
#include "ClientConsoleApp.h"
#include "api/AuditClient.h"
#include "api/AuthClient.h"
#include "api/MessagingApiClient.h"
#include "core/ClientState.h"
#include "core/HttpGatewayClient.h"
#include "core/MessagingClient.h"
#include "core/WsGatewayClient.h"
#include "core/ClientService.h"
#include <drogon/drogon.h>
#include <trantor/net/EventLoop.h>
#include <QApplication>
#include <QMetaObject>
#include <future>
#include <thread>
#include <iostream>

static MessagingClient* g_qtWsClient = nullptr;
static trantor::EventLoop* g_loop = nullptr;
static ClientService* g_service = nullptr;

static std::promise<void> g_qtReadyProm;
static std::shared_future<void> g_qtReadyFut;

static std::promise<void> g_serviceReadyProm;
static std::shared_future<void> g_serviceReadyFut;

int runQtWindow(int argc, char* argv[])
{
    QApplication app(argc, argv);

    TranslationManager translationManager(&app);
    const QString osLang = translationManager.getOsLanguage();
    translationManager.loadLanguage(osLang);

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;

    g_qtWsClient = new MessagingClient(QStringLiteral("TEMP_USER"), qApp);

    g_qtReadyProm.set_value();

    g_serviceReadyFut.wait();

    MainWindow mainWindow(g_service);
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

    auto* httpGateway = new HttpGatewayClient(httpClient, clientState);
    auto* authClient = new AuthClient(*httpGateway, clientState);
    auto* auditClient = new AuditClient(*httpGateway);
    auto* messagingApi = new MessagingApiClient(*httpGateway);

    auto* wsGateway = new WsGatewayClient(g_qtWsClient);

    g_service = new ClientService(*authClient,
                                  *auditClient,
                                  *messagingApi,
                                  wsGateway,
                                  qApp);

    g_serviceReadyProm.set_value();

    // ClientConsoleApp consoleApp(*g_service);
    // consoleApp.run();

    // QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);

    // g_loop->queueInLoop([] { g_loop->quit(); });
    loopThread.join();
}

int main(int argc, char* argv[])
{
    g_qtReadyFut = g_qtReadyProm.get_future().share();
    g_serviceReadyFut = g_serviceReadyProm.get_future().share();

    std::thread backendThread(startBackend);

    int qtRet = runQtWindow(argc, argv);

    backendThread.join();
    return qtRet;
}
