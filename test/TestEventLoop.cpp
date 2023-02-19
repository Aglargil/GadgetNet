#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <future>
#include "EventLoop.h"
#include "Common.h"

TEST(TestEventLoop, loop) {
    auto loop = std::make_unique<EventLoop>(); 
    auto loopFuture = reallyAsync([&loop]{loop->loop();});
    auto runFuture = reallyAsync(
    [&loop]{
        loop->run([]{std::cout << "run()" << std::endl;});
        // loop->quit();
        loop->quit();
    });
    loopFuture.get();
    runFuture.get();

    auto restartFuture = reallyAsync([&loop]{loop->loop();});
    restartFuture.get();

    auto closeFuture = reallyAsync([&loop]{loop->quit();});
    closeFuture.get();
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

