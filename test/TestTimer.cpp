#include <gtest/gtest.h>
#include <unistd.h>
#include <iostream>
#include "EventLoop.h"
#include "Timer.h"

TEST(TestTimer, TimerManager) {
    TimerManager tm;
    tm.addTimer(1000, []{std::cout<<"timer functor"<<std::endl;});
    tm.tick();
    sleep(1);
    tm.tick();
}

TEST(TestTimer, EventLoop) {
    EventLoopSPtr loop = EventLoop::create();

    auto print = [](std::string str){std::cout<<str<<std::endl;};
    auto timer = loop->delayRun(500, [&print]{print("first");});
    loop->dismiss(timer);
    sleep(1);
    loop->delayRun(500, [&print]{print("second");});
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

