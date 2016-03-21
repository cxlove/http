/*************************************************************************
	> File Name: http_client.h
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 一  3/14 20:23:28 2016
 ************************************************************************/

#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include "http_request.h"
#include "http_response.h"
#include "http_method.h"

class HttpClient {
public:
    HttpClient(void);
    ~HttpClient(void);

    bool Run(void);
    bool CreateSocket(void);
    bool MergeRequest(void);
    bool SendRequest(void);
    bool HandlerResponse(void);
    bool ReadResponse(void);
    bool CompleteResponse(void);

private:
    int client_sockfd_;
    int port_;
    std::string server_ip_;
    std::string remote_file_name_;
    std::string local_file_name_;
    HttpMethod method_;
    HttpRequest* http_request_;
    HttpResponse *http_response_;
    const static int kBufSize = 32;
};

#endif /* HTTP_CLIENT_H_ */
