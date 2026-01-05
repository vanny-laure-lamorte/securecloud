#include "TranslationManager.h"
#include "core/ClientState.h"
#include "core/HttpGatewayClient.h"
#include "core/WsGatewayClient.h"
#include "core/MessagingClient.h"
#include "api/AuthClient.h"
#include "api/AuditClient.h"
#include "ClientApp.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TranslationManager translationManager(&app);
    translationManager.initialize();

    std::cout << QObject::tr("K.Hello").toStdString() << std::endl;

    // --- Drogon event loop in separate thread ---
    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();

    std::thread loopThread([&loopProm]() {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    trantor::EventLoop *loop = loopFut.get();
    auto drogonHttpClient = drogon::HttpClient::newHttpClient("http://127.0.0.1:8080", loop);

    // --- Shared client state ---
    auto clientState = std::make_shared<ClientState>();

    // --- Transport HTTP ---
    HttpGatewayClient httpGateway(drogonHttpClient, clientState);

    // --- API clients ---
    AuthClient authClient(httpGateway, clientState);
    AuditClient auditClient(httpGateway);

    // --- WebSocket / Messaging ---
    std::string tempClient = "MSG_CLIENT_";
    MessagingClient messagingImpl(QString::fromStdString(tempClient));
    WsGatewayClient wsGateway(&messagingImpl);

    // --- Console app (menu CLI) ---
    ClientApp consoleApp(authClient, auditClient, &wsGateway);

    // --- Console loop in dedicated thread ---
    std::thread consoleThread([&]() {
        consoleApp.run();
        QMetaObject::invokeMethod(&app, "quit", Qt::QueuedConnection);
    });

    // --- Qt event loop ---
    int ret = app.exec();

    // --- Graceful shutdown ---
    loop->queueInLoop([loop]{ loop->quit(); });
    loopThread.join();
    consoleThread.join();

    return ret;
}
