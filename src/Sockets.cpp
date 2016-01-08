#include "Sockets.h"
#include <exception>
#include <stdexcept>

using std::cerr;
using std::endl;
using std::exception;
using std::runtime_error;

Sockets::Socket::Socket(const int &sock_fd)
{
    socket_fd = sock_fd;
    sa = nullptr;
}

Sockets::Socket::Socket(const char *host, const char *service)
{
    socket_fd = 0;
    sa = nullptr;
    addrinfo hints, *res;

    //initialiazes struct to 0
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; 

    int status;

    try
    {
        if (!(status = getaddrinfo(host, service, &hints, &res)))
        {
            //error in getting address information, throw error and log to cerr 
            //gai_strerror prints corresponding text error for getaddrinfo status
            throw runtime_error(gai_strerror(status));
        }
    }

    catch (exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    try
    {
        //loop through res linked list looking for a valid struct addrinfo *
        for (sa = res; sa != nullptr; sa = sa->ai_next)
        {
            socket_fd = socket(sa->ai_family, sa->ai_socktype, sa->ai_protocol);
            if (socket_fd != -1)
                break;
        }
        //finished looping through linked list, error if socket_fd is still -1
        //I included the for loop within the try b/c it would destroy the socket
        //in case there is an error like you say from your experiments
        if (socket_fd == -1)
        {
            throw runtime_error("No valid struct addrinfo found");
        }
    }

    catch (exception &t)
    {
        //getting the address information and creating a socket for 
        //the provided host and service failed. even if we try recreating 
        //the socket again, the same result would happen. so this thread
        //should just be skipped or ignroed by the server in a higher up code I think
        cerr << "runtime_error exception: " << t.what() << endl;
    }
}

Sockets::Socket::~Socket()
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

int Sockets::Socket::bind()
{
    //associates or binds a socket with a host and port
    //returns -1 if error in binding 
    int status;

    try 
    {
        if ((status = ::bind(socket_fd, sa->ai_addr, sa->ai_addrlen)) == -1)
        {
            throw runtime_error("Socket cannot bind to host/service");
        }
    }

    catch (exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    return status;
}

int Sockets::Socket::listen(int &backlog)
{
    //let socket listen for incoming connections
    //socket can hold and listen to 10 incoming connections in queue until a connection is accepted
    int status;

    try 
    {
        if ((status = ::listen(socket_fd, backlog)) == -1)
        {
            throw runtime_error("socket unable to listen to connection");
        }
    }

    catch(exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    return status;
}

Sockets::Socket Sockets::Socket::accept()
{
    //selects a connection from the listening queue and gives that function 
    //a new socket descriptor
    //new socket descriptor can be used in sending and receiving processes
    //sets newsocket_fd to -1 if error accepting
    int newsocket_fd;
    sockaddr_storage client_addr;
    socklen_t client_size = sizeof(client_addr);

    try
    {
        newsocket_fd = ::accept(socket_fd, (struct sockaddr *)&client_addr, &client_size);
        if (newsocket_fd == -1)
        {
            throw runtime_error("socket unable to accept connection");
        }
    }

    catch(exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    //if an exception is caught in accepting, an invalid socket with fd -1 will be returned
    //otherwise, a valid socket object with a valid fd is returned
    return Socket(newsocket_fd);
}

int Sockets::Socket::connect()
{
    int status;

    try 
    {
        if ((status = ::connect(socket_fd, sa->ai_addr, sa->ai_addrlen)) == -1)
        {
            throw runtime_error("socket unable to connect to host/service");
        }
    }

    catch(exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    return status;
}

int Sockets::Socket::send(char *buff, int &len)
{
    int bytes_sent;

    try
    {
        bytes_sent = ::send(socket_fd, buff, len, 0);
        if (bytes_sent == -1)
        {
            throw runtime_error("socket unable to send message");
        }
        if (bytes_sent != len)
        {
            throw runtime_error("socket unable to send full message");
        }
    }

    catch(exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    return bytes_sent;
}

int Sockets::Socket::recv(char *buff, int &len)
{
    int bytes_read;

    try
    {
        bytes_read = ::recv(socket_fd, buff, len, 0);
        if (bytes_read == -1)
        {
            throw runtime_error("socket unable to receive message");
        }
        if (bytes_read == 0)
        {
            throw runtime_error("sending side connection closed");
        }
    }

    catch(exception &e)
    {
        cerr << "runtime_error exception: " << e.what() << endl;
    }

    return bytes_read;
}



