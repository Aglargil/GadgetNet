#include <gtest/gtest.h>
#include "EventLoop.h"

TEST(TestEventLoop, loop) {
    auto loop = std::make_shared<EventLoop>(); 
    auto loopFuture = reallyAsync([loop]{loop->loop();});
    auto runFuture = reallyAsync(
    [loop]{
        loop->run([]{std::cout << "run()" << std::endl;});
        loop->quit();
    });
    loopFuture.get();
    runFuture.get();

    auto restartFuture = reallyAsync([loop]{loop->loop();});
    restartFuture.get();

    auto closeFuture = reallyAsync([loop]{loop->quit();});
    closeFuture.get();
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

