#pragma once

#include "EventLoopPool.h"
#include "Poller.h"
#include "TcpServer/Socket.h"
class TcpConnection {

public:
    TcpConnection(EventLoopSPtr loop, int clientFd);

private:
    EventLoopSPtr loop_;
    Socket clientSocket_;
    ChannelSPtr clientChannel_;
};