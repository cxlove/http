/*************************************************************************
	> File Name: http_method.h
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 二  3/15 19:06:36 2016
 ************************************************************************/

#ifndef HTTP_METHOD_H_
#define HTTP_METHOD_H_

#include <string>
#include "http.h"

class HttpMethod {
public:
    HttpMethod(void);
    ~HttpMethod(void);

    void Set(std::string);
    void Set(const char*, int);
    void Set(Method);

    Method Get(void) const;
    std::string GetString(void) const;
private:
    Method method_;
};

#endif /* HTTP_METHOD_H_ */
