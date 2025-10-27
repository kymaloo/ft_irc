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

#include "reply.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include <vector>

//	###	COLORS	###
#define GREY        "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define WHITE       "\033[0;37m"

/*Everything the Server needs to be set up and used.*/
class Server
{
	private:
		std::string		_serverName;
		sockaddr_in		_serverAddress;
		int				_serverSocket;
		struct pollfd	_pfds[200];
		int				_numberFds;
		Command			_cmd;

		// Setup methods

		int		setServSock();
		void	setSockAddr(int port);
		int		bindSock();
		int		listenClient(int n);

	public:
		// Setters

		void	setUpServer(int port, int n);
		int		setNewClient();
		void	unsetRevent(int i);

		// Getters

		struct pollfd*	getPfds();
		short			getRevents(int it);
		int				getNumberFds();
		int				getServSock();
		sockaddr_in&	getSockAddr();

		// Communication

		int		sendAll(char** buffer, int myself);
		void	receiveAll(char** buffer);
		int		receiveClient(char** buffer, int iterator);

		// Fds gestion

		void	compressArray();
		void	closeFd(int i);

		
		Server();
		Server(std::string& name);
		~Server();
};

//TODO: int returnValue = poll(pfds[un par fd], nb de fds, timeout)
