#include "Sockets.h"
#include <exception>

/* 
 * Dude nice! You're using sirjee! Also Nice formatting It is very pleasing to the eye lol.
 * TODO: Remove this block when we don't need int no more. 
   Questions from a programming noob that doesn't know many many things in the world. Plz answer sirjee
   Naah you're not noob, C++ is hard, like I've been saying.

   1. what is with these weird function calls? ::bind() or ::listen() like on line 89. why is there an :: before them??
      
      This is to resolve the scope. There are two functions available with the same name bind. The one defined in 
      socket.h and the one we declared in our class. The compiler will get confused as to which function to call
      and will default to look in the local scope, where it won't find a function with a matiching declaration. To
      resolve the scope we put a :: before it to tell it that it is in the global scope. Ideally, if it was a C++ STL
      function and not a good old C style system call, it would be declared in it's own namespace. (Ex. std::cout is
      declared in the namespace std.)

   2. what do you mean by logging to cerr? i didn't understand that when you said it. Or how exactly do you want
      to handle exceptions. cuz im dumb and idk what logging to cerr means lol

      'cerr' is the standard output stream for errors. Log on cerr simply means you should output the errors to
      cerr. However, as I am totally noob atm, I don't know if cerr is completely threadsafe, and we might have to 
      write a wrapper class to make cerr threadsafe so multiple threads don't compete for cerr. (Since the threads
      are being processed simultaneously, the output will be screwed up unless we make cerr thread safe lolol I
      don't know how that works, yet so for now cerr << "This is an error damn it" << endl;

   3. In the accept function, I can see that a newsocket_fd is created. however, you call Socket(newsocket_fd)
      which calls the constructor that replaces socket_fd with the newsocket_fd. Isn't this removing the old 
      socket that we still need that was listening to other connections?

      I think what I am trying to do there is create a new socket object which holds the new socket file descirptor
      returned by accept() and return this brand new object. Correct me if I'm wrong.

   4. Why is one of our private members a decltype thing? what is its purpose in our code?

      C++ is hard. At some point of time we might want to make it so that the socket can be declared as
      either a TCP or UDP socket. Since I didn't know the datatype of that SOCK_STREAM or SOCK_DGRAM
      thingies I used decltype to find it out. Not entirely necessary but cool nevertheless. 
      C++ is hard!!

   5. I still don't understand why we declared a namespace sockets and then theres a class sockets defined within 
      the namespace? what was the purpose of this as well? I thought we could just make a class and use a socket object
      while making a server? what exactly is the namespace allowing us to do?

      Namespaces are useful for avoiding name conflicts. They wrap every thing in a neat package essentially.
      Generally cool and useful. This is something you can just search online. We might want to rename our
      namespace at some point of time lol to someting like HTTP or Server or something so it covers 
      our entire program. But whatever.

   6. how you compile dis stuff without a finished makefile? g++ sockets.cpp sockets.h -o sock or something along the 
      wouldn't work because we have all these additional libraries and shit right?

      For now compiling won't do anything lol we don't have no main(). But yeah, I don't think you need additional
      libraries. Compiler will compile otherwise.
      

    Also, C++ is freaking hard dude.
*/

using std::cerr;
using std::endl;


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
	if (!(status = getaddrinfo(host, service, &hints, &res)))
	{
		//error in getting address information, throw error and log to cerr	
        //gai_strerror prints corresponding text error for getaddrinfo status
		throw std::runtime_error(gai_strerror(status));
	}

	//loop through res linked list looking for a valid struct addrinfo *
	for (sa = res; sa != nullptr; sa = sa->ai_next)
	{
		socket_fd = socket(sa->ai_family, sa->ai_socktype, sa->ai_protocol);
		if (socket_fd != -1)
			break;
	}

    // TODO: I don't know if the above loop always works. If it doesn't, then
    //       throw an exception like I have done earlier!
    
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

