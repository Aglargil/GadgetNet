#include "TcpServer/TcpServer.h"
#include "EventLoop.h"
#include "EventLoopPool.h"
#include "TcpServer/Acceptor.h"
#include "TcpServer/InetAddress.h"
#include "TcpServer/TcpConnection.h"
#include <memory>

TcpServer::TcpServer(std::string ip, uint16_t port)
    : baseLoop_(std::make_shared<EventLoop>())
    , acceptor_(baseLoop_, InetAddress(ip, port))
    , pool_(baseLoop_)
{
    acceptor_.setConnectionCallback(
    [this](int socket, const InetAddress& peerAddr)
        {newConnection(socket, peerAddr);
    });
}

TcpServer::~TcpServer() {
    
}


void TcpServer::newConnection(int socket, const InetAddress& peerAddr) {
    EventLoopSPtr subLoop = pool_.getNextLoop();
    TcpConnectionSPtr conn = std::make_shared<TcpConnection>(subLoop);
    
}

void TcpServer::start(bool isJoin) {
    static bool started = false;
    if (started) return;

    pool_.start();
    baseLoop_->run([this]{acceptor_.listen();});

    if(isJoin) {
        baseLoop_->loop();
    }
}