#include "../includes/Channel.hpp"


//---------------------------------------------------//
// Constructor & Destructor
//---------------------------------------------------//

Channel::Channel(const std::string &name, const int &fd, bool id)
{
	this->_name = name;
	this->_fdClient[fd] = id;
	this->_isPassword = false;
	this->_modes['i'] = false;
	this->_modes['t'] = false;
	this->_modes['k'] = false;
	this->_modes['l'] = false;
	this->_limit = 0;
	//? i	 invite	 drapeau de canal accessible uniquement sur invitation
	//? t	 topic	 drapeau de sujet de canal modifiable uniquement par les opérateurs
	//? k	 key	 définit la clé du canal (mot de passe)
	//? l	 limit	 définit le nombre maximal de personnes dans un canal
}

Channel::~Channel()
{

}

//---------------------------------------------------//
// Getters
//---------------------------------------------------//

void Channel::setTopic(std::string& topic)
{
	this->_topic = topic;
}

//---------------------------------------------------//
// Getters
//---------------------------------------------------//

std::string& Channel::getName()
{
	return (this->_name);
}

std::string& Channel::getTopic()
{
	return (this->_topic);
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

	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
	{
		message = Reply::RPL_PRIVMSG("prefix", "target", message);
		send(it->first, message.c_str(), message.size(), 0);
		std::cout << message << " sent to " << it->first << std::endl;
	}
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

void Channel::deleteUser(int fd)
{
	_fdClient.erase(fd);
}
