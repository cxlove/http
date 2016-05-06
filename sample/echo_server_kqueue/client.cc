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
#include <sys/socket.h>
#include <sys/event.h>
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
    char buf[1024];
    int client_fd;
    struct sockaddr_in server_addr;
    struct kevent evs[EVENT_SIZE];
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    StrToIp("127.0.0.1", &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    int kq = kqueue();
    KqueueCtl(kq, client_fd, EV_ADD);
    KqueueCtl(kq, fileno(stdin), EV_ADD);
    while(true) {
        int nfds = kevent(kq, NULL, 0, evs, EVENT_SIZE, NULL);
        if(nfds < 0) {
            cout << "kevent error" << endl;
            exit(-1);
        }
        for(int i = 0; i < nfds; ++ i) {
            if(evs[i].ident == fileno(stdin) && evs[i].filter & EVFILT_READ) {
                if(fgets(buf, 1024, stdin) == NULL) {
                    break;
                }
                else {
                    send(client_fd, buf, strlen(buf), 0);
                }
            }
            else if(evs[i].ident == client_fd && evs[i].filter & EVFILT_READ) {
                int len = recv(client_fd, buf, 1024, 0);
                if(len > 0) {
                    buf[len] = '\0';
                    fputs(buf, stdout);
                }
                else if(len == 0) {
                    cout << "server closed" << endl;
                    close(client_fd);
                    return 0;
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
