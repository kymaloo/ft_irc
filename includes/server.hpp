#pragma once

#include <errno.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <ranges>
#include <string_view>

#include "reply.hpp"
#include "Channel.hpp"
#include "client.hpp"

//	###	COLORS	###
#define GREY        "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define WHITE       "\033[0;37m"

class Command;
/*Everything the Server needs to be set up and used.*/
class Server
{
	private:
		std::string				_serverName;
		sockaddr_in				_serverAddress;

    	std::string				_pass;
		int						_serverSocket;
		struct pollfd			_pfds[200];
		int						_numberFds;
		// std::vector<Channel> 	_channels;
  		char*					_buffer;
		

		// Setup methods

		int		setServSock();
		void	setSockAddr(int port);
		int		bindSock();
		int		listenClient(int n);
		
		// Client members

		Client	clientList[200];

		//Client methods

		int				ClientNotLog(int iterator);
		int				ClientNotPass(int iterator);

		std::string		setUser(char* opt, int iterator);
		std::string		tryPass(int iterator);

		void			welcomeClient(int it);

		// Server commands methods

		std::string		nickCommand(int iterator, std::string line);
		std::string		userCommand(int iterator, std::string line);
		std::string		privmsgCommand(int iterator, std::string line);

		std::string		whichCommand(int iterator, std::string line);
		void			multipleCommands(int iterator);
	public:
		// Setters

		void	setUpServer(int port, int n);
		int		setNewClient();
		void	unsetRevent(int i);
		void	setPass(char* pass);
		void	setCommand(Command cmd);

		void	emptyBuffer();

		// Getters

		struct pollfd*	getPfds();
		short			getRevents(int it);
		int				getNumberFds();
		int				getServSock();
		sockaddr_in&	getSockAddr();
		std::string		getPass();
		std::string		&getServName();
		Command			getCommand();

		// Communication
		void	sendError(int error, int it);

		int		sendAll(char** buffer, int myself);
		void	receiveAll(char** buffer);
		int		receiveClient(char** buffer, int iterator);

		// Fds gestion

		void	compressArray();
		void	closeFd(int i);

		std::vector<Channel> 	_channels;
		Command					*_cmd;
		
		Server();
		Server(std::string& name);
		~Server();
};
