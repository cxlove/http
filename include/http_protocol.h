/*************************************************************************
	> File Name: http_protocol.h
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 二  3/15 19:38:52 2016
 ************************************************************************/

#ifndef HTTP_PROTOCOL_H_
#define HTTP_PROTOCOL_H_

#include <string>
#include "http.h"

class HttpProtocol {
public:
    HttpProtocol(void);
    ~HttpProtocol(void);

    void Set(Protocol);
    void Set(std::string);
    void Set(const char *, int);

    Protocol Get(void) const;
    std::string GetString(void) const;

private:
    Protocol protocol_;
};

#endif /* HTTP_PROTOCOL_H_ */
