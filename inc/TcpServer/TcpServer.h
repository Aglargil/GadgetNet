#pragma once

#include <unordered_map>
#include "Acceptor.h"
#include "Common.h"
#include "EventLoop.h"
#include "EventLoopPool.h"
#include "TcpConnection.h"
#include "TcpServer/InetAddress.h"


using TcpConnectionMap = std::unordered_map<std::string, TcpConnection>;

class TcpServer {

public:
    NONCOPYABLE(TcpServer)

    TcpServer(std::string ip = "127.0.0.1", uint16_t port = 51121);
    ~TcpServer();

    void newConnectionCallback(int socket, const InetAddress& peerAddr);

private:
    EventLoopSPtr baseLoop_;
    Acceptor acceptor_;
    TcpConnectionMap connectionMap_;

};