#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.h"
#include "EpollPoller.h"
#include "Common.h"

class EventLoop;

using EventLoopSPtr = std::shared_ptr<EventLoop>;
using EventLoopSPtrVector = std::vector<EventLoopSPtr>;
using initEventCallback = std::function<void(EventLoopSPtr)>;
using Functor = std::function<void()>;

class EventLoop {

public:
    NONCOPYABLE(EventLoop)

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void run(Functor cb);

    // 唤醒 EventLoop 所在线程
    void wakeup();

    // EventLoop 对象是否在自己的线程里
    bool isInThread() const { return threadPid_ == std::this_thread::get_id();}

    std::shared_ptr<Poller> getPoller() {return poller_;}

private:
    void runCallbacks();
    void handleRead();

    void invoke(Functor cb);

    std::atomic_bool looping_;
    std::atomic_bool quit_;

    const std::thread::id threadPid_;

    std::shared_ptr<Poller> poller_;

    int wakeupFd_;
    std::shared_ptr<Channel> wakeupChannel_;

    std::atomic_bool callingCb_;
    std::vector<Functor> callbackVector_;
    std::mutex mutex_;
};