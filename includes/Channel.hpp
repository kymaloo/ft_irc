#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <iostream>
# include "server.hpp"
# include <vector>
# include <sys/socket.h>

# define CHANNEL_NOT_EXIST -1
class Channel	
{
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_password;
		bool						_isPassword;
		bool						_isCreateByInvite;
		size_t						_limit;
		std::map<char, bool>		_modes;
		std::map<int, bool>			_fdClient;
		std::vector<std::string>	_invitedClient;
	private:
		
	public:
		Channel() {this->_name = "default";};
		Channel(const std::string &name, const std::string &password, const int &fd, bool id);
		Channel(const std::string &name, const int &fd, bool id);
		Channel(const std::string &name);
		~Channel();

		// Setters
		void		setTopic(std::string& topic);
		void		setMode(char mode, bool state, std::string param);
		std::string	setOperator(Server &serv, int itClient, bool state, std::vector<std::string> params);

		// Getters
		std::string&		getName();
		std::string&		getTopic();
		std::string&		getPassWorld();
		size_t&				getLimit();
		int					getSize();
		bool&				isPassWorld();
		bool				isClientOnChannel(int fd);
		bool				isClientInvited(std::string &nameClient);
		bool				isOp(int fdClient);
		bool				getMode(char mode);
		int					getLastclientFd();
		std::vector<int>	vecList();

		// Executives functions
		void	sendToChannel(int fdClient, std::string message);
		void	sendToChannelWithoutPrivateMsg(std::string message);
		void	replyToChannel(Server& serv, int rpl, std::string opt1, std::string opt2);
		void	addClient(const int &fd);
		void	deleteUser(int fd);
		void	printMap();
		void	addInvitedClient(std::string &nameClient);
		//std::map<int, bool> getFdClient();
};

#endif
