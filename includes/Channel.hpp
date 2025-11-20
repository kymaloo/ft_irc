#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <iostream>
# include "server.hpp"
# include <vector>

class Channel
{
	private:
		std::string			_name;
		std::string			_password;
		bool				_isPassword;
		std::map<int, bool>	_fdClient;
	private:
		
	public:
		Channel() {this->_name = "default";};
		Channel(const std::string &name, const int &fd, bool id);
		~Channel();

		std::string &getName();
		std::string &getPassWorld();
		bool &isPassWorld();
		void printMap();
		void addClient(const int &fd);
		void deleteUser(int fd);
		//std::map<int, bool> getFdClient();
		
};

#endif
