#include "../includes/client.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Client::Client()
{
	this->_nick = "";
	this->_user = "";
	this->_didPass = false;
}

Client::Client(Client& cl)
{
	this->_pfd = cl.getPfd();
	this->_nick = cl.getNick();
	this->_user = cl.getUser();
}

Client::~Client()
{
}


//---------------------------------------------------//
// SETTERS
//---------------------------------------------------//

void Client::setPfd(struct pollfd pollFd)
{
	this->_pfd = pollFd;
}

void Client::setNick(std::string nick)
{
	this->_nick = nick;
}

void Client::setUser(std::string user)
{
	this->_user = user;
}

void Client::setDidPass(bool pass)
{
	this->_didPass = pass;
}

Client Client::operator=(Client& cl)
{
	Client newClient;

	newClient.setPfd(cl.getPfd());
	newClient.setNick(cl.getNick());
	newClient.setUser(cl.getUser());

	return newClient;
}



//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//

struct pollfd Client::getPfd()
{
	return this->_pfd;
}

std::string Client::getNick()
{
	return this->_nick;
}

std::string Client::getUser()
{
	return this->_user;
}

bool Client::didPass()
{
	return this->_didPass;
}

//---------------------------------------------------//
// OTHER CLIENT PROCESS
//---------------------------------------------------//
