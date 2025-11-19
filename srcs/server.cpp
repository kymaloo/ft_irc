#include "../includes/server.hpp"
#include "../includes/Command.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR
//---------------------------------------------------//


Server::Server()
{
	_numberFds = 0;
	_serverName = "Default";
	_buffer = new char[1024];
	std::memset(_buffer, '\0', 1024);
	_cmd = new Command();
	for (size_t i = 0; i < 200; i++)
		_pfds[i].revents = 0;
	
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
	delete _cmd;
}



//---------------------------------------------------//
// SETTERS
//---------------------------------------------------//



// --- Server setters --- //

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

void Server::setCommand(Command cmd)
{
	this->_cmd = &cmd;
}

// --- Client setters --- //

std::string Server::setClientNick(std::string nick, int iterator)
{
	clientList[iterator].setNick(nick);
	return clientList[iterator].getNick();
}

std::string Server::setClientUser(std::string user, int iterator)
{
	clientList[iterator].setUser(user);
	return clientList[iterator].getUser();
}

std::string Server::setClientReal(std::string real, int iterator)
{
	clientList[iterator].setReal(real);
	return clientList[iterator].getReal();
}

void Server::setClientPass(bool pass, int it)
{
	this->clientList[it].setDidPass(pass);
}

void Server::setClientRegister(bool registered, int it)
{
	this->clientList[it].setDidRegister(registered);
}

// --- Channel setters --- //

void Server::setChannel(std::vector<Channel> channel)
{
	this->_channels = channel;
}

void Server::setNewUser(int it, int fd)
{
	this->_channels[it].addClient(fd);
}

void Server::setNewChannel(std::string &vecChannel, int user, bool isOp)
{
	this->_channels.push_back(Channel(vecChannel, user, isOp));
}

// ----------------------------------- //

void Server::emptyBuffer()
{
	bzero(_buffer, 1024);
}

//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//

Command& Server::getCommand()
{
	// if (_cmd)
		return (*_cmd);
}

// --- Server getters --- //

/*Access pfds.*/
struct pollfd*	Server::getPfds()
{
	return _pfds;
}

struct pollfd&	Server::getPfd(int it)
{
	return _pfds[it];
}

/*Returns the revent of [it] */
short& Server::getRevents(int it)
{
	return _pfds[it].revents;
}

/*Returns the number of open fds.*/
int&	Server::getNumberFds()
{
	return this->_numberFds;
}

/*Returns the socket's fd.*/
int& Server::getServSock()
{
	return this->_serverSocket;
}

/*Returns the server's address's structure.*/
sockaddr_in& Server::getSockAddr()
{
	return this->_serverAddress;
}

std::string& Server::getPass()
{
	return this->_pass;
}

std::string& Server::getServName()
{
	return this->_serverName;
}

// --- Client getters --- //

std::string& Server::getClientNick(int it)
{
	return this->clientList[it].getNick();
}

std::string& Server::getClientUser(int it)
{
	return this->clientList[it].getUser();
}

std::string& Server::getClientReal(int it)
{
	return this->clientList[it].getReal();
}

int& Server::getClientfd(int it)
{
	return this->clientList[it].getPfd().fd;
}

bool& Server::didClientPass(int it)
{
	return this->clientList[it].didPass();
}

bool& Server::didClientRegister(int it)
{
	return this->clientList[it].didRegister();
}

// --- Channel getters --- //

std::string& Server::getChannelName(int it)
{
	return this->_channels[it].getName();
}

std::string& Server::getPasswordChannel(size_t it)
{
	return this->_channels[it].getPassWorld();
}

bool& Server::getIsPasswordChannel(int it)
{
	return this->_channels[it].isPassWorld();
}

// Returns the amount of channels in the server
size_t Server::getChannelSize()
{
	return this->_channels.size();
}

// Returns the amount of clients in a specific channel
size_t Server::getChannelSize(int it)
{
	return this->_channels[it].getSize();
}

bool Server::isClientOnChannel(int it, int fd)
{
	return _channels[it].isClientOnChannel(fd);
}


void Server::printMapChannel(int it)
{
	this->_channels[it].printMap();
}

//---------------------------------------------------//
// CLIENT Setup Methods
//---------------------------------------------------//

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
	clientList[_numberFds].setPfd(_pfds[_numberFds]);
	
	std::cout << YELLOW << "New Client Incoming.\n" << WHITE;
	std::cout << _buffer << std::endl;

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
	std::string	returnBuffer;

	rv = recv(_pfds[iterator].fd, *buffer, 1024, 0);
	std::cout << *buffer << std::endl;
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

	_buffer = *buffer;

	while (i < rv)
		i++;
	buffer[0][i] = '\r';
	buffer[0][i + 1] = '\n';
	returnBuffer = *buffer;
  
	
	_cmd->setInput(returnBuffer);
	unsetRevent(iterator);
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

void Server::sendToChannel(int it, std::string message)
{
	if (_channels[it].getSize() > 0)
		_channels[it].sendToChannel(message);
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
				clientList[i].setNick(clientList[i + 1].getNick());
				clientList[i].setUser(clientList[i + 1].getUser());
				clientList[i].setDidPass(clientList[i + 1].didPass());
				_pfds[j].fd = _pfds[j + 1].fd;
			}
			i--;
			_numberFds--;
		}
	}
}

/*Closes a clients socket and sets its fd to -1.*/
void Server::closeFd(int i)
{
	std::cout << YELLOW << "Closing connection with client " << i << WHITE << std::endl;
	clientList[i].setNick("");
	clientList[i].setUser("");
	clientList[i].setDidPass(false);
	close(this->_pfds[i].fd);
	this->_pfds[i].fd = -1;
}

void Server::redirect(int iterator)
{
	_cmd->multiCommands(*this, iterator);
	// _cmd->redirectionCommand(*this, iterator);
}