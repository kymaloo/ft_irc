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

Client::Client(const Client& cl)
{
	this->_pfd = cl._pfd;
	this->_nick = cl._nick;
	this->_user = cl._user;
	this->_didPass = cl._didPass;
	this->_didRegister = cl._didRegister;
	this->buffer = new char[1024];
}

Client::~Client()
{
	if (buffer != NULL)
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

Client Client::operator=(const Client& cl)
{
	Client newClient;

	newClient.setPfd(cl._pfd);
	newClient.setNick(cl._nick);
	newClient.setUser(cl._user);
	newClient.setDidPass(cl._didPass);
	newClient.setDidRegister(cl._didRegister);

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
