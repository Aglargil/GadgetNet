#pragma once

#include <memory>
#include "Channel.h"
#include "Logger.h"

class Channel;
using ChannelSPtr = std::shared_ptr<Channel>;

class Poller {

public:
    Poller() {
        FUNCTION_DEBUG;
    }
    
    virtual ~Poller() {
        FUNCTION_DEBUG;
    }

    virtual void poll(int timeoutMs = 10000) = 0;

    virtual void updateChannel(ChannelSPtr channel) = 0;

    virtual void removeChannel(ChannelSPtr channel) = 0;

};