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
		std::string		_pass;

	public:
		void	setPfd(struct pollfd pollFd);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setPass(std::string pass);

		struct pollfd	getPfd();
		std::string		getNick();
		std::string		getUser();
		std::string		getPass();

		Client			operator=(Client& cl);

		Client();
		Client(Client& cl);
		~Client();
};
