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
#include <poll.h>
#include <limits.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "../../include/socklib.h"

using namespace std;
int main (int argc, char *argv[]) {
    int cnt_fd, cnt_activity;
    struct pollfd client_sockfd_status[OPEN_MAX];
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
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
    client_sockfd_status[0].fd = server_fd;
    client_sockfd_status[0].events = POLLRDNORM;
    for(int i = 1; i < OPEN_MAX; ++ i) {
        client_sockfd_status[i].fd = -1;
    }
    cnt_fd = 1;
    while(true) {
        // the last paramater is the time limit, the unit is ms
        cnt_activity = poll(client_sockfd_status, cnt_fd, -1);
        if(cnt_activity < 0) {
            cout << "poll error" << endl;
            exit(-1);
        }
        if(client_sockfd_status[0].revents & POLLRDNORM) {
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
            if(client_fd < 0) {
                cout << "accept failed" << endl;
            }
            else {
                cout << "accept the client:" << client_fd << ",ip: " << SockfdToIp(client_fd) << ",port: " << SockfdToPort(client_fd) << endl;
                cout << "the address of client: ip(" << AddrToIp(&client_addr) << "), port(" << AddrToPort(&client_addr) << ")." << endl;
                bool isfull = true;
                for(int i = 0; i < OPEN_MAX; i ++) {
                    if(client_sockfd_status[i].fd == -1) {
                        client_sockfd_status[i].fd = client_fd;
                        client_sockfd_status[i].events = POLLRDNORM;
                        isfull = false;
                        if(i + 1 >= cnt_fd) {
                            cnt_fd = i + 1;
                        }
                        break;
                    }
                }
                if(isfull) {
                    cout << "too many connections, more than " << FD_SETSIZE << endl;
                    cout << "the connection is close" << endl;
                    close(client_fd);
                    continue;
                }
                if(--cnt_activity == 0) {
                    continue;
                }
            }
        }

        for(int i = 1 ;i < cnt_fd; ++ i) {
            char buf[1024];
            if(client_sockfd_status[i].fd == -1) continue;
            if(client_sockfd_status[i].revents & (POLLERR | POLLRDNORM)) {
                int len = recv(client_sockfd_status[i].fd, buf, 1024, 0);
                buf[len] = '\0';
                if(len <= 0) {
                    if(len < 0) { 
                        cout << "read failed" << endl;
                    }
                    else {
                        cout << "client " << client_sockfd_status[i].fd << " closed" << endl;
                    }
                    close(client_sockfd_status[i].fd);
                    client_sockfd_status[i].fd = -1;
                }
                else{
                    cout << "Get data from client " << client_sockfd_status[i].fd << ": " << buf;
                    send(client_sockfd_status[i].fd, buf, len, 0);
                }
                if(--cnt_activity == 0) {
                    break;
                }
            }
        }
    }
    return 0;
}
