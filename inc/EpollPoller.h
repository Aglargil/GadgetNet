#pragma once

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>
#include "Poller.h"

using ChannelSPtrMap = std::unordered_map<int, ChannelSPtr>;
using EventVector = std::vector<epoll_event>;

class EpollPoller : public Poller {

public:
    EpollPoller();
    
    ~EpollPoller() override;

    void poll(int timeout = -1) override;

    void updateChannel(ChannelSPtr channel) override;

    void removeChannel(ChannelSPtr channel) override;

    void activeChannels(int eventsNum);

    void update(int operation, ChannelSPtr channel);
    
private:
    ChannelSPtrMap channelMap_;
    int epollFd_;
    EventVector eventVector_;
};