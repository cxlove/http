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
#include <sys/event.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../include/socklib.h"
#define EPOLL_SIZE 100
#define EVENT_SIZE 100
using namespace std;
void KqueueCtl(int kq, int sockfd, int ope) {
    struct kevent change_lists[1];
    // kevent ptr, ident, filter, flags, fflags, data, udata
    EV_SET(&change_lists[0], sockfd, EVFILT_READ, ope, 0, 0, NULL);
    if(kevent(kq, change_lists, 1, NULL, 0, NULL) < 0) {
        cout << "Kqueue CTL for fd(" << sockfd << ") failed" << endl;
        exit(-1);
    }
}
int main (int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    struct kevent evs[EVENT_SIZE];
    socklen_t len = sizeof(client_fd);
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "create socket failed" << endl;
        return 0;
    }
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
    int kq = kqueue();
    KqueueCtl(kq, server_fd, EV_ADD);

    while(true) {
        int nfds = kevent(kq, NULL, 0, evs, EVENT_SIZE, NULL);
        if(nfds < 0) {
            cout << "kevent error" << endl;
            exit(-1);
        }
        for(int i = 0; i < nfds; ++ i) {
            // kevent ptr, ident, filter, flags, fflags, data, udata
            if(evs[i].ident == server_fd && (evs[i].filter & EVFILT_READ)) {
                for(int j = 0; j < evs[i].data; ++ j) {
                    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
                    if(client_fd < 0) {
                        cout << "accept failed" << endl;
                    }
                    else {
                        cout << "accept the client:" << client_fd << ",ip: " << SockfdToIp(client_fd) << ",port: " << SockfdToPort(client_fd) << endl;
                        cout << "the address of client: ip(" << AddrToIp(&client_addr) << "), port(" << AddrToPort(&client_addr) << ")." << endl;
                        KqueueCtl(kq, client_fd, EV_ADD);
                    }
                }
            }
            else {
                int sockfd = evs[i].ident;
                char buf[1024];
                int len = recv(sockfd, buf, 1024, 0);
                if(len > 0) {
                    buf[len] = '\0';
                    cout << "Get data from client " << sockfd << ": " << buf;
                    send(sockfd, buf, len, 0);
                }
                else if(len == 0) {
                    cout << "client " << sockfd << " closed" << endl;
                    KqueueCtl(kq, sockfd, EV_DELETE);
                    close(sockfd);
                }
                else {
                    cout << "read failed" << endl;
                }
            }
        }
    }
    return 0;
}
