#include "TcpServer/TcpServer.h"
#include "EventLoop.h"
#include "EventLoopPool.h"
#include "Logger.h"
#include "TcpServer/Acceptor.h"
#include "TcpServer/InetAddress.h"
#include "TcpServer/TcpConnection.h"
#include <memory>
#include <mutex>

TcpServer::TcpServer(std::string ip, uint16_t port)
    : baseLoop_(std::make_shared<EventLoop>())
    , acceptor_(baseLoop_, InetAddress(ip, port))
    , pool_(baseLoop_)
{
    LOG_INFO(" ");
    acceptor_.setConnectionCallback(
    [this](int socket, const InetAddress& peerAddr)
        {newConnection(socket, peerAddr);
    });
    pool_.setSubLoopsNum(3);
}

TcpServer::~TcpServer() {
    LOG_INFO(" ");
}


void TcpServer::newConnection(int socket, const InetAddress& peerAddr) {
    FUNCTION_DEBUG;
    auto conn = std::make_shared<TcpConnection>(pool_.getNextLoop(), socket, peerAddr);

    conn->getLoop()->run(
    [this, conn] {
        conn->connectEstablished();

        if (connectCB_) conn->setConnectCallback(connectCB_);
        if (messageCB_) conn->setMessageCallback(messageCB_);
        if (writeCB_) conn->setWriteCallback(writeCB_);
        if (errorCB_) conn->setErrorCallback(connectCB_);

        // 设置关闭连接的 callback
        conn->setCloseCallback(
        [this](TcpConnectionSPtr conn) {
            removeConnection(conn);
            if (closeCB_) conn->setCloseCallback(closeCB_);
        });
    });

    {
        std::lock_guard<std::mutex> lock(connectionMapMutex_);
        connectionMap_[conn->getName()] = conn;
    }
}

void TcpServer::removeConnection(TcpConnectionSPtr conn) {
    FUNCTION_DEBUG;
    conn->getLoop()->run([this, conn]{conn->connectDestroyed();});
    {
        std::lock_guard<std::mutex> lock(connectionMapMutex_);
        connectionMap_.erase(conn->getName());
    }
}

void TcpServer::start() {
    static bool started = false;
    if (started) return;

    pool_.start();
    baseLoop_->run([this]{acceptor_.listen();});

    baseLoop_->loop();
}

void TcpServer::foreachConnection(const TcpConnectionCallback& cb) {
    decltype(connectionMap_) temp;
    {
        std::lock_guard<std::mutex> lock(connectionMapMutex_);
        temp = connectionMap_;
    }
    for (const auto& it : temp) {
        cb(it.second);
    }
}