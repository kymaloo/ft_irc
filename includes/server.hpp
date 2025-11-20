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
		int						_serverSocket;

    	std::string				_pass;
		struct pollfd			_pfds[200];
		int						_numberFds;
  		char*					_buffer;
		Command					*_cmd;
		std::vector<Channel> 	_channels;

		// Setup methods

		int		setServSock();
		void	setSockAddr(int port);
		int		bindSock();
		int		listenClient(int n);

		// Client members

		Client	clientList[200];
	public:
		// Setters

		void	setUpServer(int port, int n);
		int		setNewClient();
		void	unsetRevent(int i);
		void	setPass(char* pass);
		void	setCommand(Command cmd);
		void	setChannel(std::vector<Channel> channel);


		std::string	setClientNick(std::string nick, int iterator);
		std::string	setClientUser(std::string user, int iterator);
		std::string	setClientReal(std::string real, int iterator);

		void	setClientPass(bool pass, int it);
		void	setClientRegister(bool registered, int it);

		void	setNewUser(int it, int fd);
		void	setNewChannel(std::string &vecChannel, int user, bool isOp);

		void	emptyBuffer();

		// Getters

		struct pollfd*	getPfds();
		short			getRevents(int it);
		int				getNumberFds();
		int				getServSock();
		sockaddr_in&	getSockAddr();
		std::string&	getServName();
		std::string		getPass();

		std::string		getClientNick(int it);
		std::string		getClientUser(int it);
		std::string		getClientReal(int it);
		int				getClientfd(int it);

		std::string		&getChannelName(int it);
		std::string		&getPasswordChannel(size_t it);
		bool			&getIsPasswordChannel(int it);
		size_t			getChannelSize();

		void deleteUserChannel(int channel, int fd);


		void printMapChannel(int it);

		bool didClientPass(int it);
		bool didClientRegister(int it);


		Command			getCommand();

		// Communication

		int		sendAll(char** buffer, int myself);
		int		receiveClient(char** buffer, int iterator);

		// Fds gestion

		void	compressArray();
		void	closeFd(int i);

		
		

		void redirect(int iterator);

		Server();
		Server(std::string& name);
		~Server();
};
