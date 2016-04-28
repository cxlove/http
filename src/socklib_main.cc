/*************************************************************************
	> File Name: socklib_main.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 六  4/23 12:53:20 2016
 ************************************************************************/

#include <iostream>
#include <glog/logging.h>
#include <sys/socket.h>
#include "socklib.h"

int main (int agrc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold=google::INFO;
    FLAGS_colorlogtostderr=true;

    int sockfd;
    struct sockaddr_in sin;
    std::string ip = "127.0.0.1";
    sin.sin_family = AF_INET;
    sin.sin_port = htons(1024);
    StrToIp(ip.c_str(), &sin.sin_addr);
    LOG(INFO) << "Ip : "<< ip << " " << IpToStr(&sin.sin_addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr *)&sin, sizeof(sin));
    
    LOG(INFO) << "Family From Sockfd: " << SockfdToFamily(sockfd);
    LOG(INFO) << "Ip From Sockfd: " << SockfdToIp(sockfd);
    LOG(INFO) << "Port From Sockfd:" << SockfdToPort(sockfd);
    return 0;
}
