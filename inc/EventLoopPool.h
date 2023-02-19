#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include "Common.h"
#include "EventLoop.h"


using EventLoopSPtr = std::shared_ptr<EventLoop>;
using EventsubLoopVector = std::vector<EventLoopSPtr>;

using initEventCallback = std::function<void(EventLoopSPtr)>;

class EventLoopPool {

public:
    NONCOPYABLE(EventLoopPool)

    EventLoopPool(EventLoopSPtr loop);
    ~EventLoopPool();

    void start(initEventCallback cb = initEventCallback());

    void quit();

    void setSubLoopsNum(int num) {subLoopVectorNum_ = num;}

    EventLoopSPtr getNextLoop();

    bool isStarted() {return started_;}

private:
    EventLoopSPtr baseLoop_;
    
    EventsubLoopVector subLoopVector_;
    int subLoopVectorNum_;
    int nextLoopIndex_;

    bool started_;

    std::vector<std::thread> threadVector_;
};