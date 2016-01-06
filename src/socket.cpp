#include "socket.h"
using namespace std;

Socket::Socket(char * host, char * service)
{
	struct addrinfo hints;
	struct addrinfo * res;
	memset(&hints, 0, sizeof hints);             							//initialiazes struct to 0
	hints.ai_family = AF_UNSPEC;                 							//supports both IP4/6
	hints.ai_socktype = SOCK_STREAM;             							//TCP socktype
	//IDK IF WE HAVE TO SET HINTS.AI_FLAGS??
	int status = getaddrinfo(host, service, &host, &res);
	if (status != 0)
	{
		//error in getting address information, print error and exit program
		//gai_strerror prints corresponding text error for getaddrinfo status
		cerr << "Error in getaddrinfo: " << gai_strerror(status) << endl;
		exit(EXIT_FAILURE);
	}
	//loop through res linked list looking for a valid struct addrinfo *
	for (server_info = res; server_info != NULL; server_info = server_info->ai_next)
	{
		socket_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
		if (socket_fd != -1)
			break;
	}
	//server_info points to a valid struct addrinfo * 
	//socket is created and represented by the socket descriptor socket_fd
	//socket_fd and server_info can now be used in the other socket methods
}

int Socket::bind()
{
	//associates or binds a socket with a host and port
	//returns -1 if error in binding 
	return bind(socket_fd, server_info->ai_addr, server_info->ai_addrlen);
}

int Socket::listen()
{
	//let socket listen for incoming connections
	//socket can hold and listen to 10 incoming connections in queue until a connection is accepted
	//returns -1 if error in listening
	return listen(socket_fd, 10);
}

void Socket::accept()
{
	//selects a connection from the listening queue and gives that function 
	//a new socket descriptor
	//new socket descriptor can be used in sending and receiving processes
	//sets newsocket_fd to -1 if error accepting
	struct sockaddr_storage client_addr;
	socklen_t client_size = sizeof client_addr;
	newsocket_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_size)
}

int Socket::send(char * msg)
{
	//sends message to a client socket
	//returns the number of bytes of the message
	//if -1, there was an error in sending
	//if the number of bytes_sent is not equal to the number of bytes of the message
	//full message not sent, so keep sending
	int length = strlen(msg);
	int bytes_sent = send(newsocket_fd, msg, length, 0);
	if (bytes_sent == -1)
		return -1;
	if (bytes_sent != length)
	{
		while (bytes_sent != length)
		{
			
		}
	}
	return bytes_sent;
}





