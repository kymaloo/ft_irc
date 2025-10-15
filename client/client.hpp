#pragma once

#include <iostream>

#include <poll.h>

#include <unistd.h>
#include <cstdlib>
#include <vector>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class Client
{
	private:
		std::vector<int>	_clients;
		int*				_fds;
	public:
		void	newClient(int serverSocket);

		int*	getClientList();
		int		getSize();
		int		getClientFd(int iterator);

		void	setClientSock(int serverSocket, int iterator);
		void	receive(char** buffer, int iterator);
		void	receiveAll(char** buffer);

		Client();
		~Client();
};
