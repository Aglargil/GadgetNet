#pragma once

#include <functional>
#include "EventLoopPool.h"
#include "Poller.h"
#include "TcpServer/InetAddress.h"
#include "TcpServer/Socket.h"
#include "Common.h"

using ConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;

class Acceptor {

public:
    NONCOPYABLE(Acceptor);

    Acceptor(EventLoopSPtr loop, const InetAddress& listenAddr);
    ~Acceptor();

    void setConnectionCallback(ConnectionCallback cb) {connectionCB_ = cb;}

    void listen();

private:
    void handleRead();

    EventLoopSPtr loop_;

    Socket acceptSocket_;
    ChannelSPtr acceptChannel_;

    ConnectionCallback connectionCB_;
};