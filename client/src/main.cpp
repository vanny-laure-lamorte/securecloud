#include <drogon/drogon.h>
#include <trantor/net/EventLoop.h>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <string>

int main() {
    using namespace drogon;

    std::promise<trantor::EventLoop*> loopProm;
    auto loopFut = loopProm.get_future();
    std::thread loopThread([&loopProm]() {
        trantor::EventLoop loop;
        loopProm.set_value(&loop);
        loop.loop();
    });

    trantor::EventLoop* loop = loopFut.get();
    auto client = HttpClient::newHttpClient("http://127.0.0.1:8080", loop);

    for (;;) {
        std::cout << "Menu:\n"
                     "  1) Ping auth via API Gateway (/auth/ping)\n"
                     "  2) Ping messaging via API Gateway (/messaging/ping)\n"
                     "  0) Quit\n> ";
        int choice = -1;
        if (!(std::cin >> choice)) break;

        if (choice == 0) {
            std::cout << "[Client] Bye.\n";
            break;
        } else if (choice == 1) {
            auto req = HttpRequest::newHttpRequest();
            req->setMethod(Get);
            req->setPath("/auth/ping");

            auto prom = std::make_shared<std::promise<std::pair<int,std::string>>>();
            auto fut  = prom->get_future();

            client->sendRequest(req, [prom](ReqResult r, const HttpResponsePtr& resp) {
                if (r == ReqResult::Ok && resp) {
                    auto v = resp->getBody();
                    std::string body(v.data(), v.size());
                    prom->set_value({(int)resp->getStatusCode(), std::move(body)});
                } else {
                    prom->set_value({502, "bad gateway (no response)"});
                }
            });

            if (fut.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
                auto [code, body] = fut.get();
                std::cout << "[Client] " << code << " | " << body << "\n";
            } else {
                std::cout << "[Client] Timeout\n";
            }
        }
        else if (choice == 2) {
            auto req = HttpRequest::newHttpRequest();
            req->setMethod(Get);
            req->setPath("/messaging/ping");

            auto prom = std::make_shared<std::promise<std::pair<int,std::string>>>();
            auto fut  = prom->get_future();

            client->sendRequest(req, [prom](ReqResult r, const HttpResponsePtr& resp) {
                if (r == ReqResult::Ok && resp) {
                    auto v = resp->getBody();
                    std::string body(v.data(), v.size());
                    prom->set_value({(int)resp->getStatusCode(), std::move(body)});
                } else {
                    prom->set_value({502, "bad gateway (no response)"});
                }
            });

            if (fut.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
                auto [code, body] = fut.get();
                std::cout << "[Client] " << code << " | " << body << "\n";
            } else {
                std::cout << "[Client] Timeout\n";
            }
        } else {
            std::cout << "[Client] Invalid choice.\n";
        }
    }

    loop->queueInLoop([loop]{ loop->quit(); });
    loopThread.join();
    return 0;
}
