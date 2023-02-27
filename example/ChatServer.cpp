#include "TcpServer/TcpServer.h"

class ChatServer {
public:
    ChatServer(std::string ip, uint16_t port) : tcpServer_(ip, port)
    {
        tcpServer_.setSubLoopsNum(5);
        tcpServer_.setMessageCallback(
        [this](TcpConnectionSPtr conn, const std::string& msg)
            {onMessage(conn, msg);
        });
    }

    ~ChatServer() = default;

    void start() {
        tcpServer_.start();
    }

private:
    void onMessage(TcpConnectionSPtr conn, const std::string& msg) {
        LOG_INFO("%s:%s", conn->getName().c_str(), msg.c_str());
        tcpServer_.foreachConnection(
        [&msg, conn](TcpConnectionSPtr other){
            if (conn == other) return; // 过滤自己发送的消息
            other->send(other->getName() + ":" + msg);
        });
    }

    TcpServer tcpServer_;
};

int main() {
    ChatServer server("0.0.0.0", 51121);
    server.start();
}