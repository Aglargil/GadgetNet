#pragma once

#include "Common.h"
#include "EventLoop.h"

class EventLoopPool {

public:
    NONCOPYABLE(EventLoopPool)

    EventLoopPool(int subLoopVectorNum = 0, EventLoopSPtr loop = EventLoop::create());
    EventLoopPool(EventLoopSPtr loop) {EventLoopPool(0, loop);}
    ~EventLoopPool();

    void start(initEventCallback cb = initEventCallback());

    void quit();

    void setSubLoopsNum(int num) {subLoopVectorNum_ = num;}

    EventLoopSPtr getNextLoop();

    bool isStarted() {return started_;}

private:
    EventLoopSPtr baseLoop_;
    
    EventLoopSPtrVector subLoopVector_;
    int subLoopVectorNum_;
    int nextLoopIndex_;

    bool started_;

    std::vector<std::thread> threadVector_;
};