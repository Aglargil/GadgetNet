#include <cstring>
#include <unistd.h>
#include "EpollPoller.h"

EpollPoller::EpollPoller()
    : Poller()
    , epollFd_(::epoll_create(EPOLL_CLOEXEC))
    , eventVector_(16)
{
    FUNCTION_DEBUG;
    if (epollFd_ < 0) LOG_FATAL("epoll_create error:%d", errno);
}

EpollPoller::~EpollPoller() {
    FUNCTION_DEBUG;
    ::close(epollFd_);
}

void EpollPoller::poll(int timeoutMs) {
    int eventsNum = ::epoll_wait(epollFd_, &*eventVector_.begin(), static_cast<int>(eventVector_.size()), timeoutMs);
    if (eventsNum > 0) {
        LOG_DEBUG("%d events happened", eventsNum);

        activeChannels(eventsNum);

        if (eventsNum == eventVector_.size()) { // 扩容
            eventVector_.resize(eventVector_.size() * 2);
        }
    } else if (eventsNum == 0) {
        LOG_DEBUG("%s timeout!", __FUNCTION__);
    } else {
        LOG_ERROR("%s error!", __FUNCTION__);
    }
}


void EpollPoller::updateChannel(ChannelSPtr channel) {
    FUNCTION_DEBUG;
    auto status = channel->getStatus();
    switch (status) {
        case ChannelStatus::NEW: {
            channelMap_[channel->getFd()] = channel;
            // 不需要 break, 状态为 NEW 和 DELETED 的 channel 都需要 update
        }
        case ChannelStatus::DELETED: {
            channel->setStatus(ChannelStatus::ADDED);
            update(EPOLL_CTL_ADD, channel);
            break;
        }
        case ChannelStatus::ADDED: {
            if (channel->isNone()) {
                update(EPOLL_CTL_DEL, channel);
                channel->setStatus(ChannelStatus::DELETED);
            } else {
                update(EPOLL_CTL_MOD, channel);
            }
            break;
        }
        default: break;
    }
}

void EpollPoller::removeChannel(ChannelSPtr channel) {
    channelMap_.erase(channel->getFd());
    
    auto status = channel->getStatus();
    if (status == ChannelStatus::ADDED) {
        update(EPOLL_CTL_DEL, channel);
    }
    
    channel->setStatus(ChannelStatus::NEW);
}

void EpollPoller::activeChannels(int eventsNum) {
    FUNCTION_DEBUG;
    for (int i = 0; i < eventsNum; ++i) {
        Channel* channel = static_cast<Channel*>(eventVector_[i].data.ptr);
        channel->setRetEvents(eventVector_[i].events);
        channel->handleEvent();
    }
}

void EpollPoller::update(int operation, ChannelSPtr channel) {
    LOG_DEBUG("fd:%d, operation:%d", channel->getFd(), operation);
    epoll_event event;
    ::memset(&event, 0, sizeof(event));

    event.events = channel->getInEvents();
    event.data.fd = channel->getFd();
    event.data.ptr = channel.get();

    if (::epoll_ctl(epollFd_, operation, channel->getFd(), &event) < 0) {
        LOG_ERROR("epoll_ctl error:%d\n", errno);
    }
}