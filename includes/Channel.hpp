#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <iostream>
# include "server.hpp"
# include <vector>
# include <sys/socket.h>

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::string				_password;
		bool					_isPassword;
		int						_limit;
		std::map<char, bool>	_modes;
		std::map<int, bool>		_fdClient;
	private:
		
	public:
		Channel() {this->_name = "default";};
		Channel(const std::string &name, const int &fd, bool id);
		~Channel();

		// Setters
		void	setTopic(std::string& topic);

		// Getters
		std::string&	getName();
		std::string&	getTopic();
		std::string&	getPassWorld();
		int				getSize();
		bool&			isPassWorld();
		bool			isClientOnChannel(int fd);

		// Executives functions
		void	sendToChannel(std::string message);
		void	addClient(const int &fd);
		void	deleteUser(int fd);
		void	printMap();
		//std::map<int, bool> getFdClient();
};

#endif
