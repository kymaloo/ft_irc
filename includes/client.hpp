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
#include <cstring>

class Client
{
	private:
		struct pollfd	_pfd;
		std::string		_nick;
		std::string		_user;
		std::string		_real;
		bool			_didPass;
		bool			_didRegister;

	public:

		char* buffer;
		std::string sBuffer;

		// -------------------------------- //	

		void	setPfd(struct pollfd pollFd);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setReal(std::string user);
		void	setDidPass(bool pass);
		void	setDidRegister(bool registered);

		// -------------------------------- //

		const struct pollfd&	getPfd() const;
		const std::string&	getNick() const;
		const std::string&	getUser() const;
		const std::string&	getReal() const;
		const bool&			didPass() const;
		const bool&			didRegister() const;
		
		// -------------------------------- //	

		Client			operator=(const Client& cl);

		Client();
		Client(const Client& cl);
		~Client();
};
