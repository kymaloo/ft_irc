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
		std::string		_real;
		bool			_isop;
		bool			_didPass;
		bool			_didRegister;

	public:

		// -------------------------------- //	

		void	setPfd(struct pollfd pollFd);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setReal(std::string user);
		void	setOp(bool op);
		void	setDidPass(bool pass);
		void	setDidRegister(bool registered);

		// -------------------------------- //

		struct pollfd&	getPfd();
		std::string&	getNick();
		std::string&	getUser();
		std::string&	getReal();
		bool&			isop();

		bool&			didPass();
		bool&			didRegister();

		// -------------------------------- //	

		Client			operator=(Client& cl);

		Client();
		Client(Client& cl);
		~Client();
};
