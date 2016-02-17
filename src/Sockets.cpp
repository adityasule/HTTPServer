#include "Sockets.h"
#include "Error.h"
#include <exception>
#include <stdexcept>

using std::cerr;
using std::endl;

typedef Core::socket_runtime_error socket_error;
typedef Core::gai_runtime_error gai_error;

Core::Socket::Socket(const int& sock_fd)
{
    socket_fd = sock_fd;
    sa = nullptr;
}

Core::Socket::Socket(const char* host, const char* service)
{
    socket_fd = 0;
    sa = nullptr;
    addrinfo hints, *res;

    //initialiazes hints struct to 0
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; 

    int status;
    if (!(status = getaddrinfo(host, service, &hints, &res)))
    {
        //error in getting address information, throw error and log to cerr 
        //gai_strerror prints corresponding text error for getaddrinfo status
        throw gai_error(gai_strerror(status), status);
    }

    //loop through res linked list looking for a valid struct addrinfo *
    for (sa = res; sa != nullptr; sa = sa->ai_next)
    {
        socket_fd = socket(sa->ai_family, sa->ai_socktype, sa->ai_protocol);
        if (socket_fd != -1)
        {
            break;
        }
    }
    //finished looping through linked list, error if socket_fd is still -1
    //in case there is an error like you say from your experiments
    if (socket_fd == -1)
    {
        throw socket_error("No valid struct addrinfo found", errno);
    }
}

Core::Socket::~Socket()
{
    if (socket_fd)
    {
        close(socket_fd);
    }

    if (sa != nullptr)
    {
        freeaddrinfo(sa);
    }
}

void Core::Socket::bind()
{
    //associates or binds a socket with a host and port
    //returns -1 if error in binding 
    if ((:bind(socket_fd, sa->ai_addr, sa->ai_addrlen) == -1)
    {
        throw socket_error("Socket cannot bind to host/service", errno);
    }
}

void Core::Socket::listen(int &backlog)
{
    //let socket listen for incoming connections
    //socket can hold and listen to 10 incoming connections in queue until a connection is accepted
    if ((::listen(socket_fd, backlog)) == -1)
    {
        throw socket_error("socket unable to listen to connection");
    }
}

Core::Socket Core::Socket::accept()
{
    //selects a connection from the listening queue and gives that function 
    //a new socket descriptor
    //new socket descriptor can be used in sending and receiving processes
    //sets newsocket_fd to -1 if error accepting
    int newsocket_fd;
    sockaddr_storage client_addr;
    socklen_t client_size = sizeof(client_addr);

    newsocket_fd = ::accept(socket_fd, (struct sockaddr *)&client_addr, &client_size);
    if (newsocket_fd == -1)
    {
        throw socket_error("socket unable to accept connection", errno);
    }
    //if an exception is caught in accepting, an invalid socket with fd -1 will be returned
    //otherwise, a valid socket object with a valid fd is returned
    return Socket(newsocket_fd);
}

void Core::Socket::connect()
{
    if (::connect(socket_fd, sa->ai_addr, sa->ai_addrlen) == -1)
    {
        throw socket_error("socket unable to connect to host/service", errno);
    }
}

int Core::Socket::send(char *buff, int &len)
{
    int bytes_sent;

    bytes_sent = ::send(socket_fd, buff, len, 0);
    if (bytes_sent == -1)
    {
        throw socket_error("socket unable to send message", errno);
    }
    return bytes_sent;
}

int Core::Socket::recv(char *buff, int &len)
{
    int bytes_read;

        bytes_read = ::recv(socket_fd, buff, len, 0);
        if (bytes_read == -1)
        {
            throw socket_error("socket unable to receive message", errno);
        }
    return bytes_read;
}



