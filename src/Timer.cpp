#include "Timer.h"

Timer::Timer(int timeMS, Functor functor, int times) 
    : interval_(timeMS)
    , expiredTime_(Timestamp::nowAddMS(timeMS))
    , times_(1)
    , functor_(functor) 
{}

void Timer::run() {
    FUNCTION_DEBUG;
    if (times_ >= 1) {
        --times_;
        expiredTime_ = Timestamp::nowAddMS(interval_);
    } else {
        return;
    }
    if (functor_) {
        functor_();
    }
}

TimerSPtr TimerManager::addTimer(TimerSPtr timer) {
    FUNCTION_DEBUG;
    queue_.push(timer);
    return timer;
}

void TimerManager::deleteTimer(TimerSPtr timer) {
    FUNCTION_DEBUG;
    decltype(queue_) temp;
    while (!queue_.empty()) {
        auto top = queue_.top();
        queue_.pop();
        if (top != timer) {
            temp.push(top);
        }
    }
    queue_ = temp;
}

void TimerManager::tick() {
    FUNCTION_DEBUG;
    while (!queue_.empty()) {
        auto top = queue_.top();
        if (top->getExpiredTimePoint() <= Timestamp::now().getTime()) {
            top->run();
            if (top->isFinish()) {
                queue_.pop();
            }
            continue;
        }
        break;
    }
}

int TimerManager::getInterval() {
    if (queue_.empty()) {
        return -1;
    } else {
        return queue_.top()->getInterval()/10;
    }
} 
