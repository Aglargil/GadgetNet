#pragma once

#include <memory>
#include <unordered_map>
#include "Acceptor.h"
#include "Common.h"
#include "EventLoop.h"
#include "EventLoopPool.h"
#include "TcpConnection.h"
#include "TcpServer/InetAddress.h"

using TcpConnectionSPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionMap = std::unordered_map<std::string, TcpConnectionSPtr>;

class TcpServer {

public:
    NONCOPYABLE(TcpServer)

    TcpServer(std::string ip = "127.0.0.1", uint16_t port = 51121);
    ~TcpServer();

    void newConnection(int socket, const InetAddress& peerAddr);

    void setSubLoopsNum(int num) {pool_.setSubLoopsNum(num);}

    void start(bool isJoin = true);

private:
    EventLoopSPtr baseLoop_;
    Acceptor acceptor_;
    EventLoopPool pool_;

    TcpConnectionMap connectionMap_;
};