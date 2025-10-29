#include "../includes/server.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR
//---------------------------------------------------//


Server::Server()
{
	_numberFds = 0;
	_serverName = "Default";
	_buffer = new char[1024];
	std::memset(_buffer, '\0', 1024);
}

Server::Server(std::string& name)
{
	_numberFds = 0;
	_serverName = name;
	_buffer = new char[1024];
	std::memset(_buffer, '\0', 1024);
}

Server::~Server()
{
    close(_serverSocket);
	for (int i = 0; i < 199; i++)
	{
		if(_pfds[i].fd >= 0)
		close(_pfds[i].fd);
	}
	delete []_buffer;
}



//---------------------------------------------------//
// SETTERS
//---------------------------------------------------//





/*set pfds[i].revents to 0.*/
void Server::unsetRevent(int i)
{
	this->_pfds[i].revents = 0;
}

void Server::setPass(char* pass)
{
	std::string	strPass(pass);
	this->_pass = strPass;
}





//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//



/*Access pfds.*/
struct pollfd*	Server::getPfds()
{
	return _pfds;
}

/*Returns the revent of [it] */
short Server::getRevents(int it)
{
	return _pfds[it].revents;
}

/*Returns the number of open fds.*/
int	Server::getNumberFds()
{
	return this->_numberFds;
}

/*Returns the socket's fd.*/
int Server::getServSock()
{
	return this->_serverSocket;
}

/*Returns the server's address's structure.*/
sockaddr_in& Server::getSockAddr()
{
	return this->_serverAddress;
}

std::string Server::getPass()
{
	return this->_pass;
}

//---------------------------------------------------//
// CLIENT Setup Methods
//---------------------------------------------------//

void Server::welcomeClient(int it)
{
	std::string	message;
	message = Reply::RPL_WELCOME(_serverName, clientList[it].getNick(), clientList[it].getUser(), "host");
	send(_pfds[it].fd, message.c_str(), message.size(), 0);

	message = Reply::Reply::RPL_YOURHOST(_serverName, clientList[it].getNick(), "version");
	send(_pfds[it].fd, message.c_str(), message.size(), 0);

	message = Reply::RPL_CREATED(_serverName, clientList[it].getNick(), "date");
	send(_pfds[it].fd, message.c_str(), message.size(), 0);

	message = Reply::RPL_MYINFO(_serverName, clientList[it].getNick(), "version", "userModes", "channelModes");
	send(_pfds[it].fd, message.c_str(), message.size(), 0);
}

void Server::sendError(int error, int it)
{
	std::string	message;
	
	switch(error)
	{
		case 401 :
			message = Reply::ERR_NOSUCHNICK(_serverName, clientList[it].getNick(), "target");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 403 :
			message = Reply::ERR_NOSUCHCHANNEL(_serverName, clientList[it].getNick(), "channel");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 404 :
			message = Reply::ERR_CANNOTSENDTOCHAN(_serverName, clientList[it].getNick(), "channel");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 433 :
			message = Reply::ERR_NICKNAMEINUSE(_serverName, clientList[it].getNick(), "badnick");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 451 :
			message = Reply::ERR_NOTREGISTERED(_serverName, clientList[it].getNick());
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 461 :
			message = Reply::ERR_NEEDMOREPARAMS(_serverName, clientList[it].getNick(), "command");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 462 :
			message = Reply::ERR_ALREADYREGISTERED(_serverName, clientList[it].getNick());
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 421 :
			message = Reply::ERR_UNKNOWNCOMMAND(_serverName, clientList[it].getNick(), "command");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 442 :
			message = Reply::ERR_NOTONCHANNEL(_serverName, clientList[it].getNick(), "channel");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 441 :
			message = Reply::ERR_USERNOTINCHANNEL(_serverName, clientList[it].getNick(), "user", "channel");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
			return ;
		case 482 :
			message = Reply::ERR_CHANOPRIVSNEEDED(_serverName, clientList[it].getNick(), "channel");
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
		case 464 :
			message = Reply::ERR_PASSWDMISMATCH(_serverName);
			send(_pfds[it].fd, message.c_str(), message.size(), 0);
	}
}

std::string Server::setUser(char* opt)
{
	bzero(_buffer, 1024);

	recv(_pfds[_numberFds].fd, _buffer, 1024, 0);
	if ((std::strlen(_buffer) <= 5 || std::strncmp(_buffer, opt, 5) != 0) && std::strncmp(opt, "NICK ", 5) != 0)
	{
		sendError(461, _numberFds);
		return "ERROR";
	}
	else if (std::strncmp(opt, "NICK ", 5) == 0)
	{
		if (std::strlen(_buffer) <= 5 || std::strncmp(_buffer, opt, 5) != 0)
		{
			sendError(431, _numberFds);
			return "ERROR";
		}
		if (std::strlen(_buffer) > 15)
		{
			sendError(432, _numberFds);
			return "ERROR";
		}
		for (int i = 0; i < _numberFds; i++)
		{
			if (clientList[i].getNick() == std::string(_buffer).substr(5, std::strlen(_buffer) - 6))
			{
				sendError(433, _numberFds);
				return "ERROR";
			}
		}
	}

	std::string buff = _buffer;
	return buff;
}

/*Tries to accept a new client.
Sets the client address, pass, nick and username up.
Returns its fd.*/
int Server::setNewClient()
{
	int		fd = 0;

	fd = accept(_serverSocket, NULL, NULL);
	if (fd < 0)
	{
		if (errno != EWOULDBLOCK)
			std::cerr << "  accept() failed\n";
		return fd;
	}
	_pfds[_numberFds].fd = fd;
	_pfds[_numberFds].events = POLLIN;
	
	// Waiting for PASS
	
	send(_pfds[_numberFds].fd, "please use command : 'PASS password', password being the servers pass.\n", 72, 0);
	std::string buff;
	buff = setUser((char *)"PASS ");
	if (buff == "ERROR")
		return close(_pfds[_numberFds].fd);
	if (buff.substr(5, buff.size() - 6) != _pass)
	{
		sendError(464, _numberFds);
		return close(_pfds[_numberFds].fd);
	}

	// Waiting for NICK
	
	send(_pfds[_numberFds].fd, "please use command : 'NICK nickname', nickname being yours.\n", 61, 0);
	buff = setUser((char *)"NICK ");
	if (buff == "ERROR")
		return close(_pfds[_numberFds].fd);
	clientList[_numberFds].setNick(buff.substr(5, buff.size() - 6));
		
	// Waiting for USER

	send(_pfds[_numberFds].fd, "please use command : 'USER username' username being yours.\n", 60, 0);
	buff = setUser((char *)"USER ");
	if (buff == "ERROR")
		return close(_pfds[_numberFds].fd);
	clientList[_numberFds].setUser(buff.substr(5, buff.size() - 6));
	
	std::cout << GREEN << "New Client Connection.\n" << WHITE;
	welcomeClient(_numberFds);
	
	_numberFds++;
	return fd;
}



//---------------------------------------------------//
// SERVER Setup Methods
//---------------------------------------------------//



/*Creates and init a new TCP socket.
AF_INET for ipv4, SOCK_STREAM for TCP, 0 for protocol auto.*/
int Server::setServSock()
{
	return _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
}

/*Sets up the socket's address's structure.

	Family : AF_INET for ipv4.
	Port : htons(port).
	Addr : INADDR_ANY for any ip.*/
void Server::setSockAddr(int port)
{
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(port);
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
}


/*Binds server's socket to it's address.*/
int Server::bindSock()
{
    return bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
}

/*Prepare to accept connections on socket FD.
   [n] connection requests will be queued before further requests are refused.*/
int Server::listenClient(int n)
{
	return listen(_serverSocket, n);
}





/*Gives the server a socket, binds it to an address.
Also sets addresses reusable and the socket to non-blocking.*/
void Server::setUpServer(int port, int n)
{
	int rc, on = 1;
	if (setServSock() < 0)
	{
		std::cerr << "socket() failed\n";
		exit(-1);
	}
	// Sets addresses reusable.
	rc = setsockopt(_serverSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cerr << "setsockopt() failed\n";
		exit(-1);
	}
	setSockAddr(port);
	if (bindSock() < 0)
	{
		std::cerr << "bind() failed\n";
		exit(-1);
	}
	if (listenClient(n) < 0)
	{
		std::cerr << "listen() failed\n";
		exit(-1);
	}
	// Sets the socket to non-blocking,
	// so multiple clients can communicate with the server freely.
	rc = ioctl(_serverSocket, FIONBIO, (char *)&on);
	if (rc < 0)
	{
		std::cerr << "ioctl() failed\n";
		close(_serverSocket);
		exit(-1);
	}
	_pfds[0].fd = _serverSocket;
	_pfds[0].events = POLLIN;
	_numberFds++;
}





//---------------------------------------------------//
// Communication Methods
//---------------------------------------------------//


/*Fills the buffer with '\0' then recv from pfds[i].
Does not send by itself.*/
int Server::receiveClient(char** buffer, int iterator)
{
	int rv;
	int i = 0;
	std::string	returnBuffer = buffer[0];

	bzero(*buffer, 1024);

	rv = recv(_pfds[iterator].fd, *buffer, 1024, 0);
	if (rv < 0)
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "  recv() failed\n";
			exit(rv);
		}
	if (rv == 0)
	{
		std::cout << YELLOW << "  Connection closed\n" << WHITE;
		return -1;
	}
	while (i < rv)
		i++;
	buffer[0][i] = '\r';
	buffer[0][i] = '\n';
	std::cout << *buffer;
	_cmd.setInput(returnBuffer);
	return rv;
}

/*Sends the buffer to everyone except itself and the server sock
	(program stops when the server socket receive it).*/
int Server::sendAll(char** buffer, int myself)
{
	int rv = 0;
	for(int i = 1; i < _numberFds; i++)
	{
		if (i != myself)
		{
			rv = send(_pfds[i].fd, clientList[myself].getUser().c_str(), clientList[myself].getUser().size(), 0);			
			rv = send(_pfds[i].fd, " : ", 3, 0);
			rv = send(_pfds[i].fd, *buffer, strlen(*buffer), 0);
		}
		if (rv < 0)
			return -1;
	}
	return 0;
}




//---------------------------------------------------//
// Quitting Client Gestion
//---------------------------------------------------//

/*Fills the gap created by a client disconnection by moving every next client.*/
void Server::compressArray()
{
	for (int i = 0; i < _numberFds; i++)
	{
		if (_pfds[i].fd == -1)
		{
			for (int j = i; j < _numberFds-1; j++)
			{
				_pfds[j].fd = _pfds[j+1].fd;
			}
			i--;
			_numberFds--;
		}
	}
}

/*Closes a clients socket and sets its fd to -1.*/
void Server::closeFd(int i)
{
	close(this->_pfds[i].fd);
	this->_pfds[i].fd = -1;
}
