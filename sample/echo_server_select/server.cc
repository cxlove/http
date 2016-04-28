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
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "../../include/socklib.h"

using namespace std;
int main (int argc, char *argv[]) {
    int max_fd, cnt_activity;
    fd_set allset, tmpset;
    struct timeval time_limit;
    int client_sockfd_status[FD_SETSIZE];
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
    memset (client_sockfd_status, -1, sizeof (client_sockfd_status));
    FD_ZERO(&allset);
    FD_SET(server_fd, &allset);
    max_fd = server_fd + 1;
    time_limit.tv_sec = 30;
    time_limit.tv_usec = 0;
    while(true) {
        tmpset = allset;
        cnt_activity = select(max_fd, &tmpset, NULL, NULL, &time_limit);
        if(cnt_activity < 0) {
            cout << "select error" << endl;
            exit(-1);
        }
        if(FD_ISSET(server_fd, &tmpset)) {
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
            if(client_fd < 0) {
                cout << "accept failed" << endl;
            }
            else {
                cout << "accept the client:" << client_fd << ",ip: " << SockfdToIp(client_fd) << ",port: " << SockfdToPort(client_fd) << endl;
                cout << "the address of client: ip(" << AddrToIp(&client_addr) << "), port(" << AddrToPort(&client_addr) << ")." << endl;
                bool isfull = true;
                for(int i = 0; i < FD_SETSIZE; i ++) {
                    if(client_sockfd_status[i] == -1) {
                        client_sockfd_status[i] = client_fd;
                        isfull = false;
                        break;
                    }
                }
                if(isfull) {
                    cout << "too many connections, more than " << FD_SETSIZE << endl;
                    cout << "the connection is close" << endl;
                    close(client_fd);
                    continue;
                }
                if(client_fd + 1 > max_fd) {
                    max_fd = client_fd + 1;
                }
                FD_SET(client_fd, &allset);
                if(--cnt_activity == 0) {
                    continue;
                }
            }
        }

        for(int i = 0 ;i < max_fd; ++ i) {
            char buf[1024];
            if(client_sockfd_status[i] == -1) continue;
            if(FD_ISSET(client_sockfd_status[i], &tmpset)) {
                int len = recv(client_sockfd_status[i], buf, 1024, 0);
                buf[len] = '\0';
                if(len <= 0) {
                    if(len < 0) { 
                        cout << "read failed" << endl;
                    }
                    else {
                        cout << "client " << client_sockfd_status[i] << " closed" << endl;
                    }
                    FD_CLR(client_sockfd_status[i], &allset);
                    close(client_sockfd_status[i]);
                    client_sockfd_status[i] = -1;
                }
                else{
                    cout << "Get data from client:" << buf << endl;
                    send(client_sockfd_status[i], buf, len, 0);
                }
                if(--cnt_activity == 0) {
                    break;
                }
            }
        }
    }
    return 0;
}
