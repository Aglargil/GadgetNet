#include "TcpServer.h"
#include <memory>
#include "EventLoop.h"
#include "TcpServer/Acceptor.h"
#include "TcpServer/InetAddress.h"
#include "TcpServer/TcpConnection.h"


TcpServer::TcpServer(std::string ip, uint16_t port)
    : baseLoop_(std::make_shared<EventLoop>())
    , acceptor_(baseLoop_, InetAddress(ip, port))
{
    acceptor_.setConnectionCallback(
    [this](int socket, const InetAddress& peerAddr)
        {newConnectionCallback(socket, peerAddr);
    });
}

TcpServer::~TcpServer() {
    
}


void TcpServer::newConnectionCallback(int socket, const InetAddress& peerAddr) {

}