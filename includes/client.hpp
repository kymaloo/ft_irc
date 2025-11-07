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
		struct pollfd	_pfd;
		std::string		_nick;
		std::string		_user;
		bool			_didPass;

	public:

		// -------------------------------- //	

		void	setPfd(struct pollfd pollFd);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setDidPass(bool pass);

		// -------------------------------- //

		struct pollfd	getPfd();
		std::string		getNick();
		std::string		getUser();

		bool			didPass();

		// -------------------------------- //	

		Client			operator=(Client& cl);

		Client();
		Client(Client& cl);
		~Client();
};
