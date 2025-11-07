#include "../includes/Channel.hpp"

Channel::Channel(const std::string &name, const int &fd)
{
	this->_name = name;
	this->_fdClient[fd] = true;
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