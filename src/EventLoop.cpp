#include <sys/eventfd.h>
#include <unistd.h>
#include "EventLoop.h"

int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_FATAL("eventfd error:%d\n", errno);
    }
    return evtfd;
}

EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , threadPid_(std::this_thread::get_id())
    , poller_(std::make_shared<EpollPoller>())
    , wakeupFd_(createEventfd())
    , wakeupChannel_(std::make_shared<Channel>(poller_, wakeupFd_))
    , callingCb_(false)
{
    FUNCTION_DEBUG;
    wakeupChannel_->setReadCallback([this]{handleRead();});
    wakeupChannel_->enableRead();
}

EventLoop::~EventLoop() {
    FUNCTION_DEBUG;
    wakeupChannel_->remove();
    ::close(wakeupFd_);
}

void EventLoop::loop() {
    FUNCTION_DEBUG;
    looping_ = true;
    // quit_ = false;   // quit 以后就不能再进入循环了

    while (!quit_) {
        poller_->poll();
        runCallbacks();
    }
    looping_ = false;
}


void EventLoop::quit() {
    FUNCTION_DEBUG;
    quit_ = true;
    if (!isInThread()) {
        wakeup();
    }
}

// 一般是在主线程里面循环调用每个子loop的run, 即每次都不在自己的线程中, 需要invoke
void EventLoop::run(Functor cb) {
    FUNCTION_DEBUG;
    if (isInThread()) {
        cb();
    } else {
        invoke(cb);
    }
}

void EventLoop::invoke(Functor cb) {
    FUNCTION_DEBUG;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        callbackVector_.emplace_back(cb);
    }
    LOG_DEBUG("callbackVector_.size:%d", (int)callbackVector_.size());
    if (!isInThread() || callingCb_) {
        wakeup();
    }
}

void EventLoop::wakeup() {
    FUNCTION_DEBUG;
    std::uint64_t buf = 1;
    write(wakeupFd_, &buf, sizeof(buf));
}

void EventLoop::handleRead() {
    FUNCTION_DEBUG;
    std::uint64_t buf = 1;
    read(wakeupFd_, &buf, sizeof(buf));
}

void EventLoop::runCallbacks() {
    FUNCTION_DEBUG;
    decltype(callbackVector_) cbs;
    callingCb_ = true;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        cbs.swap(callbackVector_);
    }

    for (auto cb : cbs) {
        cb();
    }

    callingCb_ = false;
}
