#pragma once

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

#include <poll.h>
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*Everything the Server needs to be set up and used.*/
class Server
{
	private:
		sockaddr_in		_serverAddress;
		int				_serverSocket;
		struct pollfd	_pfds[200];

		int		setServSock();
		void	setSockAddr(int port);
		int		bindSock();
		int		listenClient(int n);
	public:
		void	setUpServer(int port, int n);

		struct pollfd*	getPfds();
		int				getServSock();
		sockaddr_in&	getSockAddr();


		Server();
		~Server();
};
