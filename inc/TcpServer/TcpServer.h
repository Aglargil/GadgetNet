#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include "Acceptor.h"
#include "Common.h"
#include "EventLoop.h"
#include "EventLoopPool.h"
#include "TcpConnection.h"
#include "TcpServer/InetAddress.h"

using TcpConnectionMap = std::unordered_map<std::string, TcpConnectionSPtr>;

class TcpServer {

public:
    NONCOPYABLE(TcpServer)

    TcpServer(std::string ip = "127.0.0.1", uint16_t port = 51121);
    ~TcpServer();

    void setSubLoopsNum(int num) {pool_.setSubLoopsNum(num);}

    void start();

    void setConnectCallback(const TcpConnectionCallback& cb) {connectCB_ = cb;}
    void setMessageCallback(const messageCallback& cb) {messageCB_ = cb;}
    void setWriteCallback(const TcpConnectionCallback& cb) {writeCB_ = cb;}
    void setCloseCallback(const TcpConnectionCallback& cb) {closeCB_ = cb;}
    void setErrorCallback(const TcpConnectionCallback& cb) {errorCB_ = cb;}

private:
    void newConnection(int socket, const InetAddress& peerAddr);

    void removeConnection(TcpConnectionSPtr conn);

    EventLoopSPtr baseLoop_;
    Acceptor acceptor_;
    EventLoopPool pool_;

    TcpConnectionMap connectionMap_;
    std::mutex connectionMapMutex_;

    TcpConnectionCallback connectCB_;
    messageCallback messageCB_;
    TcpConnectionCallback writeCB_;
    TcpConnectionCallback closeCB_;
    TcpConnectionCallback errorCB_;
};