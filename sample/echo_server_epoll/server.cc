/*************************************************************************
	> File Name: server.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 六  4/23 22:44:03 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "../../include/socklib.h"
#define EPOLL_SIZE 100
#define EVENT_SIZE 100

using namespace std;
void SetNonBlocking(int sockfd) {
    int opt;
    opt = fcntl(sockfd, F_GETFL);
    if (opt < 0) {
        cout << "fcntl(F_GETFL) fail." << endl;
        exit(-1);
    }
 
    opt |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, opt) < 0) {
        cout << "fcntl(F_SETFL) fail." << endl;
        exit(-1);
    }
}

int main (int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_fd);
    int epoll_fd = epoll_create(EPOLL_SIZE);
    struct epoll_event ev, evs[EVENT_SIZE];
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "create socket failed" << endl;
        return 0;
    }
    SetNonBlocking(server_fd);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        cout << "bind failed" << endl;
        return 0;
    }
    if(listen(server_fd, 5) < 0) {
        cout << "listen failed" << endl;
        return 0;
    }

    ev.data.fd = server_fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
    while(true) {
        // the last paramater is the time limit, the unit is ms
        int nfds = epoll_wait(epoll_fd, evs, EVENT_SIZE, -1);
        if(nfds < 0) {
            cout << "epoll error" << endl;
            exit(-1);
        }
        for(int i = 0; i < nfds; ++ i) {
            if(evs[i].data.fd == server_fd && evs[i].events & EPOLLIN) {
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
                SetNonBlocking(client_fd);
                if(client_fd < 0) {
                    cout << "accept failed" << endl;
                }
                else {
                    cout << "accept the client:" << client_fd << ",ip: " << SockfdToIp(client_fd) << ",port: " << SockfdToPort(client_fd) << endl;
                    cout << "the address of client: ip(" << AddrToIp(&client_addr) << "), port(" << AddrToPort(&client_addr) << ")." << endl;
                    ev.data.fd = client_fd;
                    ev.events = EPOLLIN | EPOLLET;
                    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                        cout << "epoll_ctl for adding failed" << endl;
                    }
                }
            }
            else if(evs[i].events & EPOLLIN) {
                int sockfd = evs[i].data.fd;
                char buf[1024];
                int len = recv(sockfd, buf, 1024, 0);
                if(len > 0) {
                    do{
                        buf[len] = '\0';
                        cout << "Get data from client " << sockfd << ": " << buf;
                        send(sockfd, buf, len, 0);
                        len = recv(sockfd, buf, 1024, 0);
                    }while(len > 0);
                }
                else if(len == 0) {
                    cout << "client " << sockfd << " closed" << endl;
                    close(sockfd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sockfd, &ev);
                }
                else if(len == EAGAIN) {
                    continue;
                }
                else {
                    cout << "read failed" << endl;
                }
            }
        }
    }
    return 0;
}
