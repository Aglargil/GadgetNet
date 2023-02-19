#pragma once

#include <atomic>
#include <ctime>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "Poller.h"
#include "EpollPoller.h"
#include "Common.h"

class Poller;
class Channel;

using Functor = std::function<void()>;

class EventLoop {

public:
    NONCOPYABLE(EventLoop)

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void run(Functor cb);
    void invoke(Functor cb);

    // 唤醒 EventLoop 所在线程
    void wakeup();

    // EventLoop 对象是否在自己的线程里
    bool isInThread() const {return threadPid_ == std::this_thread::get_id();}

    std::shared_ptr<Poller> poller() {return poller_;}

private:
    void runCallbacks();
    void handleRead();

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