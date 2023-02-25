#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstring>
#include <string>

class InetAddress {

public:
    InetAddress(std::string ip = "127.0.0.1", uint16_t port = 51121) {
        ::memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = ::htons(port);
        addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
    }
    
    InetAddress(const sockaddr_in &addr) : addr_(addr) {}
    
    ~InetAddress() = default;

    sockaddr* toSockaddr() const {
        return (sockaddr*)(&addr_);
    }

    std::string toString() const {
        char buf[64] = {0};
        ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));

        auto end = ::strlen(buf);
        auto port = ::ntohs(addr_.sin_port);
        sprintf(buf+end, ":%u", port);
        
        return buf;
    }

    void setSockAddr(sockaddr_in addr) {addr_ = addr;}
    
private:
    sockaddr_in addr_;
};