#include "TcpServer/Acceptor.h"
#include <unistd.h>
#include <memory>
#include "Poller.h"
#include "TcpServer/InetAddress.h"

static int createNonblocking()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    return sockfd;
}

Acceptor::Acceptor(EventLoopSPtr loop, const InetAddress& listenAddr)
    : loop_(loop)
    , acceptSocket_(createNonblocking())
    , acceptChannel_(Channel::create(loop_->getPoller(), acceptSocket_.getFd()))
{
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_->setReadCallback([this]{handleRead();});
}

Acceptor::~Acceptor() {
    acceptChannel_->disableAll();
    acceptChannel_->remove();
}


void Acceptor::listen() {
    acceptSocket_.listen();
    acceptChannel_->enableRead();
}

void Acceptor::handleRead() {
    InetAddress pearAddr;
    int connectionFd = acceptSocket_.accept(&pearAddr);
    if (connectionFd >= 0) {
        if (connectionCB_) {
            connectionCB_(connectionFd, pearAddr);
        } else {
            ::close(connectionFd);
        }
    }
}
