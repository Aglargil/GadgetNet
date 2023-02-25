#include "EventLoopPool.h"

EventLoopPool::EventLoopPool(EventLoopSPtr loop) 
    : baseLoop_(loop)
    , subLoopVectorNum_(0)
    , nextLoopIndex_(0)
    , started_(false)
{
    FUNCTION_DEBUG;
}

EventLoopPool::~EventLoopPool() {
    FUNCTION_DEBUG;
    for (int i = 0; i < subLoopVectorNum_; ++i) {
        subLoopVector_[i]->quit();
        threadVector_[i].join();
    }
}

void EventLoopPool::start(initEventCallback cb) {
    FUNCTION_DEBUG;
    started_ = true;

    if (subLoopVectorNum_ == 0 && cb) {
        cb(baseLoop_);
    }

    // 创建 EventLoop
    for (int i = 0; i < subLoopVectorNum_; ++i) {
        auto future = reallyAsync(
        [this, cb]{
            auto loop = std::make_shared<EventLoop>();
            if (cb) {cb(loop);}
            return loop;
        });
        subLoopVector_.emplace_back(future.get());
    }

    // EventLoop 开启 loop
    // Item 38：Be aware of varying thread handle destructor behavior
    for (const auto& loop : subLoopVector_) {
        std::packaged_task<void()> pt([&loop]{loop->loop();});
        threadVector_.emplace_back(std::move(pt));
    }
}

void EventLoopPool::quit() {
    FUNCTION_DEBUG;
    // EventLoop 退出 loop
    for (const auto& loop : subLoopVector_) {
        reallyAsync([&loop]{loop->quit();});
    }
}

EventLoopSPtr EventLoopPool::getNextLoop() {
    if ((subLoopVectorNum_ == 0) || (subLoopVector_.size() == 0)) {
        return baseLoop_;
    } else {
        if(nextLoopIndex_ >= subLoopVectorNum_) {nextLoopIndex_ = 0;}
        return subLoopVector_[nextLoopIndex_++];
    }
}
