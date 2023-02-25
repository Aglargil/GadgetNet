#pragma once

#include <unistd.h>
#include <netinet/tcp.h>
#include "InetAddress.h"
class Socket {

public:
    explicit Socket(int fd) : fd_(fd) {}
    
    ~Socket(){::close(fd_);}

    void bindAddress(const InetAddress &addr);

    void listen();

    int accept(InetAddress *outAddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on) {setSockOpt(TCP_NODELAY, on);}
    void setReuseAddr(bool on) {setSockOpt(SO_REUSEADDR, on);}
    void setReusePort(bool on) {setSockOpt(SO_REUSEPORT, on);}
    void setKeepAlive(bool on) {setSockOpt(SO_KEEPALIVE, on);}

    const int getFd() const {return fd_;}
    
private:
    void setSockOpt(int optName, bool on);
    const int fd_;
};