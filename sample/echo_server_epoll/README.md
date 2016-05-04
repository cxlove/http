# A simple echo server using epoll()

## epoll IO

1. epoll_create
2. epoll_ctl : EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL
3. epoll_wait : which files descriptor have been activated

## usage

1. make 
2. ./server 
3. ./client
