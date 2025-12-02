#include "../includes/client.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Client::Client()
{
	this->_nick = "";
	this->_user = "";
	this->_didPass = false;
	this->_didRegister = false;
	this->buffer = new char[1024];
	bzero(buffer, 1024);
}

Client::Client(Client& cl)
{
	this->_pfd = cl.getPfd();
	this->_nick = cl.getNick();
	this->_user = cl.getUser();
	this->_didPass = cl.didPass();
	this->_didRegister = cl.didRegister();
	this->buffer = new char[1024];
}

Client::~Client()
{
	delete []buffer;
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

void Client::setReal(std::string real)
{
	this->_real = real;
}

void Client::setDidPass(bool pass)
{
	this->_didPass = pass;
}

void Client::setDidRegister(bool registered)
{
	this->_didRegister = registered;
}

Client Client::operator=(Client& cl)
{
	Client newClient;

	newClient.setPfd(cl.getPfd());
	newClient.setNick(cl.getNick());
	newClient.setUser(cl.getUser());
	newClient.setDidPass(cl.didPass());
	newClient.setDidRegister(cl.didRegister());

	return newClient;
}



//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//

struct pollfd& Client::getPfd()
{
	return this->_pfd;
}

std::string& Client::getNick()
{
	return this->_nick;
}

std::string& Client::getUser()
{
	return this->_user;
}

std::string& Client::getReal()
{
	return this->_real;
}

bool& Client::didPass()
{
	return this->_didPass;
}

bool& Client::didRegister()
{
	return this->_didRegister;
}

//---------------------------------------------------//
// OTHER CLIENT PROCESS
//---------------------------------------------------//
