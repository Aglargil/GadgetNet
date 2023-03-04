#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include "Logger.h"
#include "Timestamp.h"

using Functor = std::function<void()>;

class Timer;
using TimerSPtr = std::shared_ptr<Timer>;

class Timer {

public:
    Timer(int timeMS, Functor functor, int times = 1);
    ~Timer() = default;
    static TimerSPtr create(int timeMS, Functor functor, int times = 1) {
        return std::make_shared<Timer>(timeMS, functor, times);
    }

    void run();

    const auto& getExpiredTimePoint() const {return expiredTime_.getTime();}
    const int& getInterval() const {return interval_;} 
    
    // 是否还可以执行
    bool isFinish() {return times_ < 1;}

private:
    int interval_; // ms
    Timestamp expiredTime_;
    int times_; // 还可以执行的次数
    Functor functor_;
};

class TimerManager {

public:
    TimerManager() = default;
    ~TimerManager() = default;

    TimerSPtr addTimer(TimerSPtr timer);
    TimerSPtr addTimer(int timeMS, Functor functor, int times = 1) {
        return addTimer(Timer::create(timeMS, functor, times));
    }
    
    void deleteTimer(TimerSPtr timer);

    void tick();

    // 当前tick需要等待的时间, 默认为-1; 当任务队列有timer时, 为第一个timer的interval
    // 在EventLoop调用addTimer或者deleteTimer以后会调用wakeup(), 从而在loop()中调用getInterval得到间隔时间
    int getInterval();

private:
    struct compare {
        bool operator()(const TimerSPtr t1, const TimerSPtr t2 ) {
            return t1->getExpiredTimePoint() > t2->getExpiredTimePoint();
        }
    };

    // 最小堆
    std::priority_queue<TimerSPtr, std::vector<TimerSPtr>, compare> queue_;

};