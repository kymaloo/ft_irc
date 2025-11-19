#include "../includes/Channel.hpp"


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

void Channel::addClient(const int &fd)
{
	this->_fdClient[fd] = false;
}

std::string Channel::getName()
{
	return (this->_name);
}

std::string Channel::getPassWorld()
{
	return (this->_password);
}

bool Channel::isPassWorld()
{
	return (this->_isPassword);
}

void Channel::printMap()
{
	std::map<int, bool>::iterator it;
	for (it = _fdClient.begin(); it != _fdClient.end(); it++)
	{
		std::cout << it->first << " => " << it->second << std::endl;
	}
}

// std::map<int, bool> Channel::getFdClient()
// {
// 	return (this->_fdClient);
// }
