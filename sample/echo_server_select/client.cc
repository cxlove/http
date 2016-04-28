/*************************************************************************
	> File Name: client.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 六  4/23 22:43:55 2016
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../include/socklib.h"

using namespace std;
int main (int argc, char *argv[]) {
    char buf[1024];
    struct timeval time_limit;
    fd_set allset;
    int max_fd;
    int client_fd;
    struct sockaddr_in server_addr;
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    StrToIp("127.0.0.1", &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    max_fd = max(client_fd, fileno(stdin)) + 1;
    FD_ZERO(&allset);
    FD_SET(client_fd, &allset);
    FD_SET(fileno(stdin), &allset);
    time_limit.tv_sec = 30;
    time_limit.tv_usec = 0;
    while(true) {
        FD_SET(client_fd, &allset);
        FD_SET(fileno(stdin), &allset);
        int cnt_activity;
        if((cnt_activity = select(max_fd, &allset, NULL, NULL, &time_limit)) < 0) {
            cout << "select failed" << endl;
            continue;
        }
        if(FD_ISSET(client_fd, &allset)) {
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
        if(FD_ISSET(fileno(stdin), &allset)) {
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
