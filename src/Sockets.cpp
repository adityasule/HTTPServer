#include "Sockets.h"

Sockets::Socket::Socket()
{
    sock_fd = 0;
}

Sockets::Socket::Socket(int sock_fd)
{
    sock_fd = 0;
}

int Sockets::Socket::open(char *hostname, char *service)
{
    // Don't know if this works lol but I just wanted to start with some code
    addrinfo hints, *res;
    
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(hostname, service, &hints, &res);
    sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sock_fd, res->ai_addr, res->ai_addrlen);

    return 0;
}
