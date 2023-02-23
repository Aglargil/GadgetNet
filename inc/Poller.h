#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Common.h"
#include "Logger.h"

class Channel;

using ChannelSPtr = std::shared_ptr<Channel>;
using ChannelSPtrVector = std::vector<ChannelSPtr>;

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