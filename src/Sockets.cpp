#include "Sockets.h"
#include <exception>


using std::cerr;
using std::endl;

/* 
   Questions from a programming noob that doesn't know many many things in the world. Plz answer sirjee
   1. what is with these weird function calls? ::bind() or ::listen(). why is there an :: before them??
   2. what do you mean by logging to cerr? i didn't understand that when you said it. Or how exactly do you want
      to handle exceptions. cuz im dumb and idk what logging to cerr means lol
   3. In the accept function, I can see that a newsocket_fd is created. however, you call Socket(newsocket_fd)
      which calls the constructor that replaces socket_fd with the newsocket_fd. Isn't this removing the old 
      socket that we still need that was listening to other connections?
   4. Why is one of our private members a decltype thing? what is its purpose in our code?
   5. I still don't understand why we declared a namespace sockets and then theres a class sockets defined within 
      the namespace? what was the purpose of this as well? I thought we could just make a class and use a socket object
      while making a server? what exactly is the namespace allowing us to do?
   6. how you compile dis stuff without a finished makefile? g++ sockets.cpp sockets.h -o sock or something along the 
      wouldn't work because we have all these additional libraries and shit right?
*/

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

	try
	{
		if (!(status = getaddrinfo(host, service, &hints, &res)))
		{
			//error in getting address information, throw error and log to cerr
			//gai_strerror prints corresponding text error for getaddrinfo status
			throw runtime_error(gai_strerror(status));
		}
	}

	catch (exception & e)
	{
		//log to cerr how exactly? or what exactly do we do here?
	}

	//loop through res linked list looking for a valid struct addrinfo *
	for (sa = res; sa != nullptr; sa = sa->ai_next)
	{
		socket_fd = socket(sa->ai_family, sa->ai_socktype, sa->ai_protocol);
		if (socket_fd != -1)
			break;
	}
	//sa points to a valid struct addrinfo * 
	//socket is created and represented by the socket descriptor socket_fd
	//socket_fd and sa can now be used in the other socket methods
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
	// TODO: Exceptions Exceptions!
	// TODO: This functon call is bad!!

    return ::bind(socket_fd, sa->ai_addr, sa->ai_addrlen);
}

int Sockets::Socket::listen(int &backlog)
{
	//let socket listen for incoming connections
	//socket can hold and listen to 10 incoming connections in queue until a connection is accepted
	//returns -1 if error in listening
	// TODO: Freaking Exceptions maaaaan!

	return ::listen(socket_fd, backlog);
}

Sockets::Socket Sockets::Socket::accept()
{
	//selects a connection from the listening queue and gives that function 
	//a new socket descriptor
	//new socket descriptor can be used in sending and receiving processes
	//sets newsocket_fd to -1 if error accepting
	sockaddr_storage client_addr;
	socklen_t client_size = sizeof(client_addr);
	int newsocket_fd = ::accept(socket_fd, (struct sockaddr *)&client_addr, &client_size);

	return Socket(newsocket_fd);
}

// This function is completely broken fix!
// Also freaking indent it right.
/*int Sockets::Socket::send()
{
//sends message to a client socket
//returns the number of bytes of the message
//if -1, there was an error in sending
//if the number of bytes_sent is not equal to the number of bytes of the message
//full message not sent, so keep sending

// Since this is a threaded application this function should not block thread at this level
// I think it is best to return the length of bytes written
// TODO: Throw dem exceptions son!

int length = strlen(msg);

// this line will no longer compile!
//int bytes_sent = send(newsocket_fd, msg, length, 0);
int bytes_sent = 0;
if (bytes_sent == -1)
return -1;
if (bytes_sent != length)
{
while (bytes_sent != length)
{
//keep sending until full msg sent
//delete bytes_sent number of bytes from beginning of message and send remaining message?
}
}
return bytes_sent;
}*/





