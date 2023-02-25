#include <TcpServer/TcpServer.h>
#include <cstdint>
#include "Logger.h"
#include "TcpServer/TcpConnection.h"

class EchoServer {
public:
    EchoServer(std::string ip, uint16_t port) : tcpServer_(ip, port)
    {
        tcpServer_.setSubLoopsNum(5);
        tcpServer_.setMessageCallback(
        [this](TcpConnectionSPtr conn, const std::string& msg)
            {onMessage(conn, msg);
        });

    }

    ~EchoServer() = default;

    void start() {
        tcpServer_.start();
    }

private:
    void onMessage(TcpConnectionSPtr conn, const std::string& msg) {
        LOG_INFO("%s:%s", conn->getName().c_str(), msg.c_str());
        conn->send("echo");
    }

    TcpServer tcpServer_;
};

int main() {
    EchoServer server("0.0.0.0", 51121);
    server.start();
}