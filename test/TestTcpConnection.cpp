#include <gtest/gtest.h>
#include <memory>
#include "EventLoop.h"
#include "TcpServer/InetAddress.h"
#include "TcpServer/TcpConnection.h"

using EventLoopSPtr = std::shared_ptr<EventLoop>;
using TcpConnectionSPtr = std::shared_ptr<TcpConnection>;

TEST(TcpConnectionTest, main) {
    EventLoopSPtr loop = std::make_shared<EventLoop>();
    InetAddress addr;
    TcpConnectionSPtr conn = std::make_shared<TcpConnection>(loop, 0, addr);
    conn->connectEstablished();
    conn->connectDestroyed();
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

