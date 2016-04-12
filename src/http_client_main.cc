/*************************************************************************
	> File Name: http_client_main.cc
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 一  3/21 10:18:22 2016
 ************************************************************************/

#include <glog/logging.h>
#include "http_client.h"

int main (int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold=google::INFO;
    FLAGS_colorlogtostderr=true;

    HttpClient *http_client;
    if(argc == 3) {
        for(int i = 0; i < 100; i ++) {
            char str[10];sprintf(str, "%d", i);
            std::string path = argv[2] + std::string(str) + std::string(".html");
            http_client = new HttpClient(argv[1], path);
            http_client->Run();
            delete http_client;
        }
    }
    else {
        LOG(ERROR) << "Invalid paramenters!";
        return 0;
    }
    return 0;
}
