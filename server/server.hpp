#pragma once

#include <errno.h>

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <poll.h>
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


/*Everything the Server needs to be set up and used.*/
class Server
{
	private:
		sockaddr_in					_serverAddress;
		int							_serverSocket;
		struct pollfd				_pfds[200];
		int							_numberFds;

		int		setServSock();
		void	setSockAddr(int port);
		int		bindSock();
		int		listenClient(int n);

	public:
		void		setUpServer(int port, int n);
		int			setNewClient();
		
		void		sendAll(char** buffer, int myself);
		void		receiveAll(char** buffer);
		int			receiveClient(char** buffer, int iterator);

		struct pollfd*	getPfds();
		int				getNumberFds();
		int				getServSock();
		sockaddr_in&	getSockAddr();

		Server();
		~Server();
};

//TODO: int returnValue = poll(pfds[un par fd], nb de fds, timeout)
