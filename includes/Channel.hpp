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
		int							_limit;
		std::map<char, bool>		_modes;
		std::map<int, bool>			_fdClient;
		std::vector<std::string>	_invitedClient;
	private:
		
	public:
		Channel() {this->_name = "default";};
		Channel(const std::string &name, const int &fd, bool id);
		Channel(const std::string &name);
		~Channel();

		// Setters
		void	setTopic(std::string& topic);
		void	setMode(char mode, bool state, std::string param);
		void	setOperator(Server &serv, bool state, std::vector<std::string> params);

		// Getters
		std::string&	getName();
		std::string&	getTopic();
		std::string&	getPassWorld();
		int				getSize();
		bool&			isPassWorld();
		bool			isClientOnChannel(int fd);
		bool			isClientInvited(std::string &nameClient);
		bool			isOp(int fdClient);
		bool			getMode(char mode);

		// Executives functions
		void	sendToChannel(std::string message);
		void	replyToChannel(Server& serv, int rpl, std::string opt1, std::string opt2);
		void	addClient(const int &fd);
		void	deleteUser(int fd);
		void	printMap();
		void	addInvitedClient(std::string &nameClient);
		//std::map<int, bool> getFdClient();
};

#endif
