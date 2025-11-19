#include "../includes/Channel.hpp"


//---------------------------------------------------//
// Constructor & Destructor
//---------------------------------------------------//

Channel::Channel(const std::string &name, const int &fd, bool id)
{
	this->_name = name;

	if (id == true)
		this->_fdClient[fd] = true;
	else
		this->_fdClient[fd] = false;
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
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
		send(it->first, message.c_str(), message.size(), 0);
}

void Channel::addClient(const int &fd)
{
	this->_fdClient[fd] = false;
}

void Channel::printMap()
{
	if (_fdClient.empty())
	{
		std::cout << "Empty printMap\n";
		return ;
	}
	std::cout << "In channel " << _name << " (" << _fdClient.size() << ") :\n";
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
		std::cout << it->first << " => " << it->second << std::endl;
}
