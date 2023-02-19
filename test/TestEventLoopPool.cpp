#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include "Common.h"
#include "EventLoop.h"
#include "EventLoopPool.h"
#include "Logger.h"

using EventLoopSPtr = std::shared_ptr<EventLoop>;
using EventLoopPoolSPtr = std::shared_ptr<EventLoopPool>;

class TestEventLoopPool : public testing::Test {

protected:
    EventLoopSPtr mainLoop_;
    EventLoopPoolSPtr pool_;
};

TEST_F(TestEventLoopPool, oneLoop) {
    mainLoop_ = std::make_shared<EventLoop>();
    pool_ = std::make_shared<EventLoopPool>(mainLoop_);
    pool_->start();
    pool_->quit();
}

TEST_F(TestEventLoopPool, multiLoop) {
    mainLoop_ = std::make_shared<EventLoop>();
    pool_ = std::make_shared<EventLoopPool>(mainLoop_);
    
    reallyAsync(
    [this]{
        pool_->setSubLoopsNum(3);
        pool_->start();
    });

    reallyAsync([this]{pool_->quit();});
}
int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

