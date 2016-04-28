/*************************************************************************
	> File Name: socklib.h
	> Author: cxlove
	> Mail: cxlove321@gmail.com
	> Created Time: 五  4/22 22:40:06 2016
 ************************************************************************/

#ifndef SOCKLIB_H_
#define SOCKLIB_H_

#include <string>
#include <netinet/in.h>

/* get the socket address according to the sockfd */
struct sockaddr_in* SockfdToAddr(int);
/* get the address family according to the sockfd */
int SockfdToFamily(int);
/* get the ip address according to the sockfd */
std::string SockfdToIp(int);
/* get the port according to the sockfd */
int SockfdToPort(int);
/* convert ip address from string */
int StrToIp(const char *, struct in_addr *);
int StrToIp(std::string, struct in_addr *);
/* convert ip address into string */
std::string IpToStr(struct in_addr*);

#endif /* SOCKLIB_H_ */
