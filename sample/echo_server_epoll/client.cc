/*************************************************************************
	> File Name: client.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 六  4/23 22:43:55 2016
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <poll.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
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
    char buf[1024];
    int client_fd;
    struct sockaddr_in server_addr;
    int epoll_fd = epoll_create(EPOLL_SIZE);
    struct epoll_event ev, evs[EVENT_SIZE];
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    StrToIp("127.0.0.1", &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    SetNonBlocking(client_fd);
    ev.data.fd = client_fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
    ev.data.fd = fileno(stdin);
    ev.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fileno(stdin), &ev);

    while(true) {
        int nfds = epoll_wait(epoll_fd, evs, EVENT_SIZE, -1);
        if(nfds < 0) {
            cout << "epoll error" << endl;
            exit(-1);
        }
        for(int i = 0; i < nfds; ++ i) {
            if(evs[i].data.fd == fileno(stdin) && evs[i].events & EPOLLIN) {
                if(fgets(buf, 1024, stdin) == NULL) {
                    break;
                }
                else {
                    send(client_fd, buf, strlen(buf), 0);
                }
            }
            if(evs[i].data.fd == client_fd && evs[i].events & EPOLLIN) {
                int len = recv(client_fd, buf, 1024, 0);
                if(len > 0) {
                    do {
                        buf[len] = '\0';
                        fputs(buf, stdout);
                        len = recv(client_fd, buf, 1024, 0);
                    }while(len > 0);
                }
                else if(len == 0) {
                    cout << "server closed" << endl;
                    close(client_fd);
                    return 0;
                }
                else if(len == EAGAIN) {
                    continue;
                }
                else {
                    cout << "recv failed" << endl;
                    exit(-1);
                }
            }
        }
    }
    return 0;

}
