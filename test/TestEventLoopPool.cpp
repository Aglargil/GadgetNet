#include <gtest/gtest.h>
#include "EventLoopPool.h"

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

    auto func = []{LOG_DEBUG("testRun()");};

    for (int i = 0; i < 10; ++i) {
        pool_->getNextLoop()->run(func);
    }

    reallyAsync([this]{pool_->quit();});
}
int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

