/*************************************************************************
	> File Name: http_status.h
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 三  3/16 18:43:36 2016
 ************************************************************************/

#ifndef HTTP_STATUS_H_
#define HTTP_STATUS_H_

#include <string>

class HttpStatus {
public:
    HttpStatus(void);
    ~HttpStatus(void);

    bool SetStatusCode(int);
    int GetStatusCode(void) const;
    bool SetReasonPhrase(void);
    bool SetReasonPhrase(std::string);
    std::string GetReasonPhrase(void) const;
    
private:
    int status_code_;
    std::string reason_phrase_;
};

#endif /* HTTP_STATUS_H_ */
