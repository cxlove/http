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
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../include/socklib.h"

using namespace std;
int main (int argc, char *argv[]) {
    char buf[1024];
    struct pollfd poll_fd[2];
    int client_fd;
    struct sockaddr_in server_addr;
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    StrToIp("127.0.0.1", &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    poll_fd[0].fd = client_fd;
    poll_fd[0].events = POLLRDNORM;
    poll_fd[1].fd = fileno(stdin);
    poll_fd[1].events = POLLRDNORM;

    while(true) {
        int cnt_activity;
        if((cnt_activity = poll(poll_fd, 2, -1)) < 0) {
            cout << "poll failed" << endl;
            continue;
        }
        if(poll_fd[0].revents & POLLRDNORM) {
            int len = recv(client_fd, buf, 1024, 0);
            if(len < 0) {
                cout << "recv failed" << endl;
                break;
            }
            if(len == 0) {
                cout << "server closed" << endl;
                break;
            }
            buf[len] = '\0';
            fputs(buf, stdout);
        }
        if(poll_fd[1].revents & POLLRDNORM) {
            if(fgets(buf, 1024, stdin) == NULL) {
                break;
            }
            else {
                send(client_fd, buf, strlen(buf), 0);
            }
        }
    }
    return 0;
}
