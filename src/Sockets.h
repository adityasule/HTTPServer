#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace Sockets
{
    /**
     * The main socket class. Each object behaves like a unique pointer to constructed socket.
     * I wonder if this is a good approach? Also RAII!
     * At some point of time, we will have to make this class atomic, or write an atomic wrapper
     * , or maybe not? I don't know this shit yet.
     * on top of this class.
     * TODO: Add documentation to this file. Follow javadoc style (like this comment).
     */
    class Socket
    {
        public:
            /**
             * Default Constructor. Add Doc
             */
            Socket();

            /**
             * Constructor to construct object with sockfd
             */
            Socket(int sock_fd);

            // TODO: Add constructors for IP address and or hostnames!

            /**
             * Default Destructor. Add Doc
             */
            ~Socket();

            // These are just functions I think this class will need. Remove comment when you fix this!
            int read(char *buff, int len);
            int write();
            int listen();
            int accept();
            void bind();

            /**
             * Because RAII, this one is a maybe. Also if we do have it, then the socket should turn
             * useless after it is called. Who knows man.
             */
            void close();
        private:
            int sock_fd;
    };

}

#endif
// sockets.h
