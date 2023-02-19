#include <gtest/gtest.h>
#include <future>
#include <iostream>
#include <memory>
#include "Channel.h"
#include "EpollPoller.h"
#include "Poller.h"

class TestEpollPoller : public testing::Test {

protected:
    void SetUp() override {
        poller_ = std::make_shared<EpollPoller>();

        int fd_ = -1;
        channel_ = std::make_shared<Channel>(poller_, fd_);
    }

    std::shared_ptr<Poller> poller_;
    
    int fd_;
    std::shared_ptr<Channel> channel_;
};


TEST_F(TestEpollPoller, poll) {
    poller_->poll(1);
}

TEST_F(TestEpollPoller, updateChannel) {
    EXPECT_EQ(channel_->getStatus(), ChannelStatus::NEW);
    poller_->updateChannel(channel_);
    EXPECT_EQ(channel_->getStatus(), ChannelStatus::ADDED);

    poller_->updateChannel(channel_);
    EXPECT_EQ(channel_->getStatus(), ChannelStatus::DELETED);
}

TEST_F(TestEpollPoller, removeChannel) {
    poller_->removeChannel(channel_);
    EXPECT_EQ(channel_->getStatus(), ChannelStatus::NEW);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

