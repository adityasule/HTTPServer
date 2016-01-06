#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

namespace Sockets
{
	class Socket
	{
		public:

			Socket(char * host, char * service);
			~Socket();
			int bind();
			int listen();
			void accept();
			int send(char * msg);

		private:

			int socket_fd;
			struct addrinfo * server_info;
			int newsocket_fd;
	};
}

#endif