#pragma once

#include <sys/epoll.h>
#include <functional>
#include <memory>
#include "Common.h"
#include "Logger.h"
#include "Poller.h"

class Poller;

using PollerWPtr = std::weak_ptr<Poller>;

// enum class EpollEvent {
//     NONE  = 0,
//     READ  = EPOLLIN | EPOLLPRI,
//     WRITE = EPOLLOUT
// };
// 强枚举类型无法使用位运算
struct EpollEvent {
    enum Value {
        NONE = 0,
        READ = EPOLLIN | EPOLLPRI,
        WRITE = EPOLLOUT,
        CLOSE = EPOLLHUP,
        ERROR = EPOLLERR
    };
};

enum class ChannelStatus {
    NEW,     // 未添加到 Poller 的 channel
    ADDED,   // 在 Poller 中, 且设置了 EPOLL_CTL_ADD 的 channel
    DELETED  // 在 Poller 中, 且设置了 EPOLL_CTL_DEL 的 channel
};

using EventCallback = std::function<void()>;

// Channel 和 fd 一一对应
class Channel : public std::enable_shared_from_this<Channel> {

public:
    NONCOPYABLE(Channel)

    Channel(PollerWPtr poller, int fd);

    ~Channel() {
        FUNCTION_DEBUG;
    }

    void handleEvent();

    void update();

    void remove();

    // 返回fd当前的事件状态
    bool isNone() {return inEvents_ == EpollEvent::NONE;}
    bool isReading() {return inEvents_ & EpollEvent::READ;}
    bool isWriting() {return inEvents_ & EpollEvent::WRITE;}

    // 设置fd相应的事件状态 相当于epoll_ctl add delete
    void enableReading() { FUNCTION_DEBUG; inEvents_ |= EpollEvent::READ; update(); }
    void disableReading() { FUNCTION_DEBUG; inEvents_ &= ~EpollEvent::READ; update(); }
    void enableWriting() { FUNCTION_DEBUG; inEvents_ |= EpollEvent::WRITE; update(); }
    void disableWriting() { FUNCTION_DEBUG; inEvents_ &= ~EpollEvent::WRITE; update(); }
    void disableAll() { FUNCTION_DEBUG; inEvents_ = EpollEvent::NONE; update(); }

    const int fd() const {return fd_;}

    int inEvents() {return inEvents_;}
    void setRetEvents(int events) {retEvents_ = events;}

    ChannelStatus getStatus() {return status_;}
    void setStatus(ChannelStatus status) {status_ = status;}

    // 设置回调函数对象
    void setReadCallback(EventCallback cb) { readCB_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCB_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCB_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCB_ = std::move(cb); }

private:
    PollerWPtr poller_;
    const int fd_;
    int inEvents_;   // 注册fd感兴趣的事件
    int retEvents_;  // Poller返回的具体发生的事件
    ChannelStatus status_;

    EventCallback readCB_;
    EventCallback writeCB_;
    EventCallback closeCB_;
    EventCallback errorCB_;
};