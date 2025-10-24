#include "../includes/client.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Client::Client()
{
}

Client::Client(Client& cl)
{
	this->_pfd = cl.getPfd();
	this->_nick = cl.getNick();
	this->_user = cl.getUser();
	this->_pass = cl.getPass();
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

void Client::setPass(std::string pass)
{
	this->_pass = pass;
}


Client Client::operator=(Client& cl)
{
	Client newClient;

	newClient.setPfd(cl.getPfd());
	newClient.setNick(cl.getNick());
	newClient.setUser(cl.getUser());
	newClient.setPass(cl.getPass());

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

std::string Client::getPass()
{
	return this->_pass;
}


//---------------------------------------------------//
// OTHER CLIENT PROCESS
//---------------------------------------------------//
