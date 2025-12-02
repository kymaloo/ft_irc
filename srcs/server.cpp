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
	for (int i = 0; i < _numberFds; i++)
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

void Server::setClientNick(std::string nick, int iterator)
{
	clientList[iterator].setNick(nick);
}

void Server::setClientUser(std::string user, int iterator)
{
	clientList[iterator].setUser(user);
}

void Server::setClientReal(std::string real, int iterator)
{
	clientList[iterator].setReal(real);
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

// Adds a client to a channel
void Server::setNewUser(int it, int fd)
{
	this->_channels[it].addClient(fd);
}

void Server::setNewChannel(std::string &name, int fdClient, bool isOp)
{
	this->_channels.push_back(Channel(name, fdClient, isOp));
}

void Server::setChannelTopic(int channelIt, std::string& topic)
{
	this->_channels[channelIt].setTopic(topic);
}

void Server::setChannelTopic(std::string& channelName, std::string& topic)
{
	this->_channels[getChannelIterator(channelName)].setTopic(topic);
}

void Server::setChannelMode(char mode, bool state, int itChannel, std::string param)
{
	this->_channels[itChannel].setMode(mode, state, param);
}

std::string Server::setChannelOperators(bool state, int itClient, int itChannel, std::vector<std::string> params)
{
	return this->_channels[itChannel].setOperator(*this, itClient, state, params);
}

// ----------------------------------- //

void Server::emptyBuffer()
{
	bzero(_buffer, 1024);
}

//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//

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

int Server::getClientIt(int fd)
{
    for (int i = 0; i < _numberFds; i++)
        if (this->_pfds[i].fd == fd)
            return i;
    return -1;
}

int& Server::getClientfd(int it)
{
	return this->clientList[it].getPfd().fd;
}

int Server::getClientfd(std::string clientNick)
{
	for (int i = 0; i < getNumberFds(); i++)
		if (getClientNick(i) == clientNick)
			return getClientfd(i);
	return -1;
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

bool Server::doesChannelExist(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return true;
	}
	return false;
}

bool Server::doesChannelExist(size_t it)
{
	return (it < _channels.size());
}

std::string& Server::getChannelName(int it)
{
	return this->_channels[it].getName();
}

std::string& Server::getChannelTopic(int it)
{
	return this->_channels[it].getTopic();
}

std::string& Server::getChannelTopic(std::string& name)
{
	return this->_channels[getChannelIterator(name)].getTopic();
}

size_t Server::getChannelIterator(std::string& name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return i;
	}
	return 0;
}

std::string& Server::getPasswordChannel(size_t it)
{
	return this->_channels[it].getPassWorld();
}

int& Server::getChannelLimit(size_t it)
{
	return this->_channels[it].getLimit();
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

bool Server::isOpInChannel(int i, int fdClient)
{
	std::cout << "fdclient : " << fdClient << "Is Op : " << this->_channels[i].isOp(fdClient) << std::endl;
	return this->_channels[i].isOp(fdClient);
}

bool Server::getChannelMode(char mode, int itChannel)
{
	return this->_channels[itChannel].getMode(mode);
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
	
	std::cout << YELLOW << "New Client Incoming in fd " << _pfds[_numberFds].fd << WHITE << std::endl;
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
int Server::receiveClient(int iterator)
{
	int rv;
	int i = 0;
	std::string	returnBuffer;

	rv = recv(_pfds[iterator].fd, _buffer, 1024, 0);
	std::cout << _buffer << std::endl;
	if (rv < 0)
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "  recv() failed\n";
			exit(rv);
		}
	if (rv == 0)
		return -1;
	while (i < rv)
		i++;
	returnBuffer = _buffer;
	returnBuffer += "\r\n";
  
	_cmd->setInput(returnBuffer);
	unsetRevent(iterator);
	return rv;
}

void Server::redirect(int iterator)
{
	_cmd->multiCommands(*this, iterator);
	// _cmd->redirectionCommand(*this, iterator);
}

void Server::sendToChannel(int it, std::string message)
{
	if (_channels[it].getSize() > 0)
		_channels[it].sendToChannel(message);
}

void Server::replyToChannel(int itChannel, int rpl, std::string opt1, std::string opt2)
{
	if (_channels[itChannel].getSize() > 0)
		_channels[itChannel].replyToChannel(*this, rpl, opt1, opt2);
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
void Server::closeFd(int itClient)
{
	std::cout << YELLOW << "Closing connection with client " << itClient << WHITE << std::endl;
	clientList[itClient].setNick("");
	clientList[itClient].setUser("");
	clientList[itClient].setDidPass(false);
	close(this->_pfds[itClient].fd);
	this->_pfds[itClient].fd = -1;
}

void Server::deleteUserChannel(int i, int fdClient)
{
	this->_channels[i].deleteUser(fdClient);
}
