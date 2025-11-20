#include "../includes/Channel.hpp"


//---------------------------------------------------//
// Constructor & Destructor
//---------------------------------------------------//

Channel::Channel(const std::string &name, const int &fd, bool id)
{
	this->_name = name;
	this->_fdClient[fd] = id;
	this->_isPassword = false;
}

Channel::~Channel()
{

}

//---------------------------------------------------//
// Getters
//---------------------------------------------------//

std::string& Channel::getName()
{
	return (this->_name);
}

std::string& Channel::getPassWorld()
{
	return (this->_password);
}

int Channel::getSize()
{
	return _fdClient.size();
}

bool& Channel::isPassWorld()
{
	return (this->_isPassword);
}

bool Channel::isClientOnChannel(int fd)
{
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
		if (it->first == fd)
			return true;
	return false;
}

//---------------------------------------------------//
// Executives functions
//---------------------------------------------------//

void Channel::sendToChannel(std::string message)
{
	std::cout << "the message to send is :" << message << ": and its size is :" << message.size() << std::endl;
	char* msg = new char[message.size()];
	for (size_t i = 0; i < message.size(); i++)
		msg[i] = message[i];
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
	{
		send(it->first, msg, message.size(), 0);
		std::cout << msg << " sent to " << it->first << std::endl;
	}
	delete []msg;
}

void Channel::addClient(const int &fd)
{
	this->_fdClient[fd] = false;
}

void Channel::printMap()
{
	if (_fdClient.empty())
	{
		std::cout << "Empty printMap" << std::endl;
		return ;
	}
	std::cout << "In channel " << _name << " (" << _fdClient.size() << ") :\n";
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
		std::cout << it->first << " => " << it->second << std::endl;
}
