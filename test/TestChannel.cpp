#include <gtest/gtest.h>
#include <sys/eventfd.h>
#include "Channel.h"
#include "EpollPoller.h"

class TestChannel : public testing::Test {
protected:

    void SetUp() override {
        poller_ = std::make_shared<EpollPoller>();

        fd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        channel_ = Channel::create(poller_, fd_);

        EXPECT_EQ(channel_->isNone(), true);

    }

    void TearDown() override {
        channel_->remove();
        ::close(fd_);
    }

    std::shared_ptr<Poller> poller_;

    int fd_;
    std::shared_ptr<Channel> channel_;

    std::future<void> future;
};

TEST_F(TestChannel, update) {
    channel_->enableRead();
    EXPECT_EQ(channel_->isReadable(), true);
    EXPECT_EQ(channel_->isNone(), false);
}

TEST_F(TestChannel, handleEvent) {
    channel_->handleEvent();
}

TEST_F(TestChannel, event){
        FUNCTION_DEBUG;

        future = reallyAsync([this]{poller_->poll();  });
        
        channel_->setReadCallback([this]{
            std::uint64_t one = 1;
            ::read(fd_, &one, sizeof(one));
        });

        channel_->enableRead();
        reallyAsync([this]{ 
            std::uint64_t buf = 1; 
            write(fd_, &buf, sizeof(buf));
        });
        
        future.get();
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

