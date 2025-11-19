#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <iostream>

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

		std::string getName();
		std::string getPassWorld();
		bool isPassWorld();
		void printMap();
		void addClient(const int &fd);
		//std::map<int, bool> getFdClient();
		
};

#endif
