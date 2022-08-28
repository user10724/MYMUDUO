#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
//封装socket地址类型
using namespace std;
class InetAddress
{
public:
    explicit InetAddress(uint16_t port,string ip ="127.0.0.1");
    explicit InetAddress(const sockaddr_in &addr)
    :addr_(addr)
    {}
    string toIp()   const;
    string toIpPort() const;
    uint16_t toPort() const;

    const sockaddr_in* getSockAddr() const {
        return &addr_;
    }
private:
    sockaddr_in addr_;
};