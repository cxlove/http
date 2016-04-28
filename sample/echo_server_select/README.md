# A simple echo server using select IO

## select IO

When a client connect to server, it will be blocked in IO. If the server closed in the same time, client can't receive the FIN because it's blocked.

## usage

1. make 
2. ./server 
3. ./client
