/*************************************************************************
	> File Name: http_server.h
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 四  3/10 20:09:11 2016
 ************************************************************************/

#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "http_request.h"
#include "http_response.h"


class HttpServer {
public:
    HttpServer(void);
    HttpServer(int);
    ~HttpServer(void);

    bool SetPort(int);
    bool Run(void);
    bool CreateSocket(void);
    bool StartListening(void);
    bool HandleRequest(void);
    bool ReadRequest(void);
    bool GenerateResponse(void);
    bool HandlerGetRequest(void);
    bool MergeResponse(void);
    bool SendResponse(void); 

private:
    int port_;
    int server_sockfd_;
    int client_sockfd_;
    HttpRequest *http_request_;
    HttpResponse *http_response_;
    static const int kDefaultPort = 1024;
    static const int kBufSize = 32;
};

#endif /* HTTP_SERVER_H_ */
