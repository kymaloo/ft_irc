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
		Command					*_cmd;
		std::vector<Channel> 	_channels;
		Client*					_clientList;

		// Setup methods

		int		setServSock();
		void	setSockAddr(int port);
		int		bindSock();
		int		listenClient(int n);

		void	redirect(int iterator);
		void	clearBuffer(int iterator);
	public:
		// Setters
		// Server setters
		void	setUpServer(int port, int n);
		int		setNewClient();
		void	unsetRevent(int i);
		void	setPass(char* pass);
		void	setCommand(Command cmd);
		void	setChannel(std::vector<Channel> channel);

		// Client setters
		void	setClientNick(std::string nick, int iterator);
		void	setClientUser(std::string user, int iterator);
		void	setClientReal(std::string real, int iterator);
		void	setClientPass(bool pass, int it);
		void	setClientRegister(bool registered, int it);


		// Channel setters
		void		setNewUser(int it, int fd);
		void		setNewChannel(std::string &name, int user, bool isOp);
		void		setNewChannel(std::string &name);
		void		setChannelTopic(int channelIt, std::string& topic);
		void		setChannelTopic(std::string& channelName, std::string& topic);
		void		setChannelMode(char mode, bool state, int itChannel, std::string param);
		std::string	setChannelOperators(bool state, int itClient, int itChannel, std::vector<std::string> params);

		
		// Getters
		// Server getters
		struct pollfd*	getPfds();
		struct pollfd&	getPfd(int it);
		short&			getRevents(int it);
		int&			getNumberFds();
		int&			getServSock();
		sockaddr_in&	getSockAddr();
		std::string&	getServName();
		std::string&	getPass();

		// Client getters
		std::string&	getClientNick(int it);
		std::string&	getClientUser(int it);
		std::string&	getClientReal(int it);
		int&			getClientfd(int it);
		int				getClientfd(std::string clientNick);
		int				getClientIt(int fd);
		bool&			didClientPass(int it);
		bool&			didClientRegister(int it);

		// Channel getters
		std::string&	getChannelName(int it);
		std::string&	getChannelTopic(int it);
		std::string&	getChannelTopic(std::string& name);
		size_t			getChannelIterator(std::string& name);
		std::string&	getPasswordChannel(size_t it);
		int&			getChannelLimit(size_t it);
		bool&			getIsPasswordChannel(int it);
		bool			isClientOnChannel(int it, int fd);
		size_t			getChannelSize();
		size_t			getChannelSize(int it);
		bool			doesChannelExist(std::string name);
		bool			doesChannelExist(size_t it);
		bool			isOpInChannel(int i, int fdClient);
		bool			getChannelMode(char mode, int itChannel);
		std::vector<std::string> vecListChannelName(int it);

		// Communication
		void	sendToChannel(int it, std::string message);
		void 	sendToChannelWithoutPrivateMsg(int it, std::string message);
		void	replyToChannel(int itChannel, int rpl, std::string opt1, std::string opt2);
		int		receiveClient(int iterator);

		// Fds gestion
		void	compressArray();
		void	closeFd(int i);

		void	printMapChannel(int it);
		void	deleteUserChannel(int i, int it);
		void	addChannelInvitedClient(std::string &name, int i, int fdClient);
		bool	isClientInvitedInChannel(int itChannel, int itClient);

		//Constructor & Destructor
		Server();
		Server(std::string& name);
		~Server();
};
