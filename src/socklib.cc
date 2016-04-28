/*************************************************************************
	> File Name: socklib.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 五  4/22 22:24:39 2016
 ************************************************************************/

#include <iostream>
#include <string>
#include <glog/logging.h>
#include <arpa/inet.h>
#include "socklib.h"

struct sockaddr_in* SockfdToAddr(int sockfd) {
    struct sockaddr_in* sin = new sockaddr_in();
    socklen_t len = sizeof(*sin);
    if(getsockname(sockfd, (struct sockaddr*) sin, &len) < 0) {
        LOG(ERROR) << "Failed to get address according to the sockfd";
        return NULL;
    }
    return sin;
}

int SockfdToFamily(int sockfd) {
    struct sockaddr_in *sin = SockfdToAddr(sockfd);
    if(NULL == sin) {
        return -1;
    }
    return sin->sin_family;
}

std::string SockfdToIp(int sockfd) {
    struct sockaddr_in *sin = SockfdToAddr(sockfd);
    if(NULL == sin) {
        return "";
    }
    return inet_ntoa(sin->sin_addr); 
}

int SockfdToPort(int sockfd) {
    struct sockaddr_in *sin = SockfdToAddr(sockfd);
    if(NULL == sin) {
        return -1;
    }
    return ntohs(sin->sin_port);
}

int StrToIp(const char *str, struct in_addr *addr) {
    return inet_pton(AF_INET, str, (void *)addr);
}

int StrToIp(std::string str, struct in_addr *addr) {
    return StrToIp(str.c_str(), addr);
}

std::string IpToStr(struct in_addr *addr) {
    char str[20];
    inet_ntop(AF_INET, (void *)addr, str, 16);
    return std::string(str);
}
