#include "Channel.h"

Channel::Channel(PollerWPtr poller, int fd)
    : poller_(poller)
    , fd_(fd)
    , inEvents_(EpollEvent::NONE)
    , retEvents_(EpollEvent::NONE)
    , status_(ChannelStatus::NEW)
{
    FUNCTION_DEBUG;
}

void Channel::update() {
    FUNCTION_DEBUG;
    auto poller = poller_.lock();
    if (poller) {
        poller->updateChannel(shared_from_this());
    }
}

void Channel::remove() {
    auto poller = poller_.lock();
    if (poller) {
        poller->removeChannel(shared_from_this());
    }
}

void Channel::handleEvent() {
    FUNCTION_DEBUG;
    if (readCB_ && (retEvents_ & EpollEvent::READ)) {
        readCB_();
    }
    if (writeCB_ && (retEvents_ & EpollEvent::WRITE)) {
        writeCB_();
    }
    if (errorCB_ && (retEvents_ & EpollEvent::ERROR)) {
        errorCB_();
    }
    if (closeCB_ && (retEvents_ & EpollEvent::CLOSE) && !(retEvents_ & EpollEvent::READ)) {
        closeCB_();
    }
}