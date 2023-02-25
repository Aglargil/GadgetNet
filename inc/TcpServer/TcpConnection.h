#pragma once

#include <functional>
#include "EventLoopPool.h"
#include "Poller.h"
#include "TcpServer/InetAddress.h"
#include "TcpServer/Socket.h"

class TcpConnection;

using TcpConnectionSPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(TcpConnectionSPtr)>;
using messageCallback = std::function<void(TcpConnectionSPtr, const std::string&)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {

public:
    TcpConnection(EventLoopSPtr loop, int clientFd, const InetAddress& clientAddr);
    ~TcpConnection();

    void connectEstablished();
    void connectDestroyed();
    
    void send(const std::string& msg);

    void shutdownWrite();

    void setConnectCallback(const TcpConnectionCallback& cb) {connectCB_ = cb;}
    void setMessageCallback(const messageCallback& cb) {messageCB_ = cb;}
    void setWriteCallback(const TcpConnectionCallback& cb) {writeCB_ = cb;}
    void setCloseCallback(const TcpConnectionCallback& cb) {closeCB_ = cb;}
    void setErrorCallback(const TcpConnectionCallback& cb) {errorCB_ = cb;}

    std::string getName() const {return clientAddr_.toString();}
    EventLoopSPtr getLoop() const {return loop_;}

private:

    // 设置 Channel 的四个 Callback, 会在 loop 中执行
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    void sendMessage(const char* data, size_t len);

    EventLoopSPtr loop_;
    Socket clientSocket_;
    ChannelSPtr clientChannel_;
    InetAddress clientAddr_;

    TcpConnectionCallback connectCB_;
    messageCallback messageCB_;
    TcpConnectionCallback writeCB_;
    TcpConnectionCallback closeCB_;
    TcpConnectionCallback errorCB_;

    enum class State {
        CONNECTING,
        CONNECTED,
        DISCONNECTING,
        DISCONNECTED
    };

    State state_;
};