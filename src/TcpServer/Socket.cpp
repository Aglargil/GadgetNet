#include "TcpServer/Socket.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include "Logger.h"


void Socket::bindAddress(const InetAddress& addr) {
    if (::bind(fd_, addr.toSockaddr(), sizeof(sockaddr_in)) == -1) {
        FUNCTION_DEBUG;
    }
}

void Socket::listen() {
    if (::listen(fd_, 1024) == -1) {
        FUNCTION_DEBUG;
    }     
}

int Socket::accept(InetAddress* outAddr) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    ::memset(&addr, 0, len);
    // 将 connectionFd 设置为非阻塞
    int connectionFd = ::accept4(fd_, (sockaddr*)&addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connectionFd >= 0) {
        if (outAddr) {outAddr->setSockAddr(addr);}
    }
    return connectionFd;
}

void Socket::shutdownWrite() {
    if (::shutdown(fd_, SHUT_WR) == -1) {
        FUNCTION_DEBUG;
    }
}

void Socket::setSockOpt(int optName, bool on) {
    ::setsockopt(fd_, IPPROTO_TCP, optName, &on, sizeof(on));
}
