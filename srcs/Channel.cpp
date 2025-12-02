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
	this->_isCreateByInvite = false;
	//? i	 invite	 drapeau de canal accessible uniquement sur invitation
	//? t	 topic	 drapeau de sujet de canal modifiable uniquement par les opérateurs
	//? k	 key	 définit la clé du canal (mot de passe)
	//? l	 limit	 définit le nombre maximal de personnes dans un canal
}

Channel::Channel(const std::string &name)
{
	this->_name = name;
	this->_isPassword = false;
	this->_modes['i'] = false;
	this->_modes['t'] = false;
	this->_modes['k'] = false;
	this->_modes['l'] = false;
	this->_limit = 0;
	this->_isCreateByInvite = true;
}

Channel::~Channel()
{

}

//---------------------------------------------------//
// Setters
//---------------------------------------------------//

void Channel::setTopic(std::string& topic)
{
	this->_topic = topic;
}

void Channel::setMode(char mode, bool state, std::string param)
{
	this->_modes[mode] = state;
	switch (mode)
	{
		case 'i' | 't':
			this->_modes[mode] = state;
			return;
		case 'l':
			if (state == true)
				this->_limit = std::atoi(param.c_str());
			else
				this->_limit = 0;
			return;
		case 'k':
			if (state == true)
				this->_password = param;
			else
				this->_password = "";
			return;
	}
}

std::string Channel::setOperator(Server &serv, int itClient, bool state, std::vector<std::string> params)
{
	size_t size = params.size();
	std::string operators;
	int fdClient = -1;

	if (size > 3)
		size -= size - 3;
	for (size_t itParams = 0; itParams < size; itParams++)
	{
		fdClient = serv.getClientfd(params[itParams]);
		if (fdClient != -1 && isClientOnChannel(fdClient) == true)
			_fdClient[fdClient] = state;
		else
			Reply::sendError(serv, 401, itClient, params[itParams], "NULL");
		operators += params[itParams];
		if (itParams < size - 1)
			operators += ",";
	}
	return operators;
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

int& Channel::getLimit()
{
	return (this->_limit);
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

bool Channel::isOp(int fdClient)
{
	return _fdClient[fdClient];
}

bool Channel::getMode(char mode)
{
	return this->_modes[mode];
}

//---------------------------------------------------//
// Executives functions
//---------------------------------------------------//

void Channel::sendToChannel(std::string message)
{
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
	{
		message = Reply::RPL_PRIVMSG("prefix", "target", message);
		send(it->first, message.c_str(), message.size(), 0);
	}
}

void Channel::replyToChannel(Server& serv, int rpl, std::string opt1, std::string opt2)
{
	for (std::map<int, bool>::iterator it = _fdClient.begin(); it != _fdClient.end(); it++)
		Reply::sendReply(serv, rpl, serv.getClientIt(it->first), opt1, opt2);
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

void Channel::addInvitedClient(std::string &nameClient)
{
	_invitedClient.push_back(nameClient);
}

bool Channel::isClientInvited(std::string &nameClient)
{
	for (size_t i = 0; i != _invitedClient.size(); i++)
		if (_invitedClient[i] == nameClient)
			return true;
	return false;
}