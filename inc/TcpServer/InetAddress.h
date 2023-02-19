#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
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

    void setSockAddr(sockaddr_in addr) {addr_ = addr;}
    
private:
    sockaddr_in addr_;
};