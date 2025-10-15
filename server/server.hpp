#pragma once

#include <iostream>

#include <poll.h>

#include <unistd.h>
#include <cstdlib>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*Everything the Server needs to be set up and used.*/
class Server
{
	private:
		sockaddr_in	_serverAddress;
		int			_serverSocket;
	public:
		void	setServSock();
		void	setSockAddr(int port);
		int				getServSock();
		sockaddr_in&	getSockAddr();

		void	bindSock();
		void	listenClient(int n);

		Server();
		~Server();
};
