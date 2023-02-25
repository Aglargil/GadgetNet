#include <unistd.h>
#include <string>
#include "Channel.h"
#include "Logger.h"
#include "TcpServer/TcpConnection.h"

TcpConnection::TcpConnection(EventLoopSPtr loop, int clientFd, const InetAddress& clientAddr) 
    : loop_(loop)
    , clientSocket_(clientFd)
    , clientChannel_(std::make_shared<Channel>(loop_->getPoller(), clientFd))
    , clientAddr_(clientAddr)
    , state_(State::CONNECTING)
{
    FUNCTION_DEBUG;
    clientChannel_->setReadCallback([this]{handleRead();});
    clientChannel_->setWriteCallback([this]{handleWrite();});
    clientChannel_->setCloseCallback([this]{handleClose();});
    clientChannel_->setErrorCallback([this]{handleError();});

    clientSocket_.setKeepAlive(true); 

    LOG_INFO("name:%s, fd:%d", getName().c_str(), clientFd);
}

TcpConnection::~TcpConnection() {
    FUNCTION_DEBUG;
    if (state_ != State::DISCONNECTED) connectDestroyed();
    LOG_INFO("name:%s", getName().c_str());
}

void TcpConnection::connectEstablished() {
    FUNCTION_DEBUG;
    state_ = State::CONNECTED;

    clientChannel_->enableRead();

    if (connectCB_) {
        loop_->run([this]{connectCB_(shared_from_this());});
    }
}

void TcpConnection::connectDestroyed() {
    FUNCTION_DEBUG;
    state_ = State::DISCONNECTED;

    clientChannel_->disableAll();
    clientChannel_->remove();
}

void TcpConnection::send(const std::string& msg) {
    FUNCTION_DEBUG;
    if (state_ != State::CONNECTED) return;

    loop_->run([this, &msg]{sendMessage(msg.c_str(), msg.length());});
}

void TcpConnection::sendMessage(const char* data, size_t len) {
    // 在 loop 执行 sendMessage 之前有可能连接已经断开了
    if (state_ != State::CONNECTED) {
        LOG_ERROR("connection is not connected, send message \"%s\" fail", data);
    }

    auto n = ::write(clientChannel_->getFd(), data, len);
    if (n >= 0) {
        if (writeCB_) {
            loop_->run([this]{writeCB_(shared_from_this());});
        }
    } else {
        if (errno != EWOULDBLOCK) {
            LOG_ERROR("send message fail");
        }
    }
}

void TcpConnection::shutdownWrite() {
    if (state_ != State::CONNECTED) return;

    state_ = State::DISCONNECTING;
    clientSocket_.shutdownWrite();
}

void TcpConnection::handleRead() {
    char buf[65536] = {0}; 
    auto n = ::read(clientChannel_->getFd(), buf, sizeof(buf));
    if (n > 0) {
        if (messageCB_) {
            loop_->run([this, &buf]{messageCB_(shared_from_this(), buf);});
        }
    } else {
        handleClose();
    }
}

void TcpConnection::handleWrite() {
    ;
}

void TcpConnection::handleClose() {
    state_ = State::DISCONNECTED;

    if (closeCB_) {
        loop_->run([this]{closeCB_(shared_from_this());});
    }
}

void TcpConnection::handleError() {
    LOG_ERROR("name:%s", getName().c_str());
}
