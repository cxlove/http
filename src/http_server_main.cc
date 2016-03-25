/*************************************************************************
	> File Name: http_server_main.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 五  3/11 15:04:35 2016
 ************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <glog/logging.h>
#include "http_server.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold=google::INFO;
    FLAGS_colorlogtostderr=true;
    
    LOG(INFO) << "HttpServerMain Begin!";
    int port;
    HttpServer *http_server;
    if(argc > 1) {
        sscanf(argv[1], "%d", &port);
        http_server = new HttpServer(port);
    }
    else {
        http_server = new HttpServer();
    }
    http_server->Run();

    delete http_server;
    return 0;
}
