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
	this->_pfd = cl.getPfd();
	this->_nick = cl.getNick();
	this->_user = cl.getUser();
	this->_didPass = cl.didPass();
	this->_didRegister = cl.didRegister();
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
	this->_pfd = cl.getPfd();
	this->_nick = cl.getNick();
	this->_user = cl.getUser();
	this->_didPass = cl.didPass();
	this->_didRegister = cl.didRegister();

	return *this;
}



//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//

const struct pollfd& Client::getPfd() const
{
	return this->_pfd;
}

const std::string& Client::getNick() const
{
	return this->_nick;
}

const std::string& Client::getUser() const
{
	return this->_user;
}

const std::string& Client::getReal() const
{
	return this->_real;
}

const bool& Client::didPass() const
{
	return this->_didPass;
}

const bool& Client::didRegister() const
{
	return this->_didRegister;
}

//---------------------------------------------------//
// OTHER CLIENT PROCESS
//---------------------------------------------------//
