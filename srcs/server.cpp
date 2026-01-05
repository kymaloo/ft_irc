#include "../includes/server.hpp"
#include "../includes/Command.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR
//---------------------------------------------------//


Server::Server()
{
	_numberFds = 0;
	_serverName = "ft_irc";
	_version = "PIv1.0.0";
	_clientList.push_back(Client());
	_cmd = new Command();
}

Server::Server(std::string& name)
{
	_numberFds = 0;
	_serverName = name;
	_version = "PIv1.0.0";
	_clientList.push_back(Client());
	_cmd = new Command();
}

Server::~Server()
{
    close(_serverSocket);

	// for (int i = 0; i < _numberFds; i++)
	// {
	// 	if(_pfds[i].fd >= 0)
	// 		close(_pfds[i].fd);
	// }
	delete _cmd;
}



//---------------------------------------------------//
// SETTERS
//---------------------------------------------------//



// --- Server setters --- //

/*set pfds[i].revents to 0.*/
void Server::unsetRevent(int i)
{
	this->_clientList[i].unsetRevent();
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
	_clientList[iterator].setNick(nick);
}

void Server::setClientUser(std::string user, int iterator)
{
	_clientList[iterator].setUser(user);
}

void Server::setClientReal(std::string real, int iterator)
{
	_clientList[iterator].setReal(real);
}

void Server::setClientPass(bool pass, int it)
{
	this->_clientList[it].setDidPass(pass);
}

void Server::setClientRegister(bool registered, int it)
{
	this->_clientList[it].setDidRegister(registered);
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

void Server::setNewChannel(std::string &name, std::string &password, int fdClient, bool isOp)
{
	this->_channels.push_back(Channel(name, password, fdClient, isOp));
}

void Server::setNewChannel(std::string &name, int fdClient, bool isOp)
{
	this->_channels.push_back(Channel(name, fdClient, isOp));
}

void Server::setNewChannel(std::string &name)
{
	this->_channels.push_back(Channel(name));
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

//---------------------------------------------------//
// GETTERS
//---------------------------------------------------//

// --- Server getters --- //

struct pollfd&	Server::getPfd(int it)
{
	return _clientList[it].getPfd();
}

/*Returns the revent of [it] */
const short& Server::getRevents(int it)
{
	return _clientList[it].getRevent();
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

std::string& Server::getVersion()
{
	return this->_version;
}

// --- Client getters --- //

const std::string& Server::getClientNick(int it) const
{
	return this->_clientList[it].getNick();
}

const std::string& Server::getClientUser(int it) const
{
	return this->_clientList[it].getUser();
}

const std::string& Server::getClientReal(int it) const
{
	return this->_clientList[it].getReal();
}

int Server::getClientIt(int fd)
{
    for (int i = 0; i < _numberFds; i++)
        if (this->_clientList[i].fd() == fd)
            return i;
    return -1;
}

const int& Server::getClientfd(int it) const
{
	return this->_clientList[it].fd();
}

int Server::getClientfd(std::string clientNick)
{
	for (int i = 0; i < getNumberFds(); i++)
		if (getClientNick(i) == clientNick)
			return getClientfd(i);
	return -1;
}

const bool& Server::didClientPass(int it) const
{
	return this->_clientList[it].didPass();
}

const bool& Server::didClientRegister(int it) const
{
	return this->_clientList[it].didRegister();
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

size_t& Server::getChannelLimit(size_t it)
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

std::vector<std::string> Server::vecListChannelName(int it)
{
	std::vector<int> vec = this->_channels[it].vecList();
	std::vector<std::string> result;

	for (size_t i = 0; i != vec.size(); i++)
		result.push_back(getClientNick(getClientIt(vec[i])));
	return result;
}

bool Server::isClientInvitedInChannel(int itChannel, int itClient)
{
	std::string name = getClientNick(itClient);
	return this->_channels[itChannel].isClientInvited(name);
}

int Server::getChannelLastclientFd(int itChannel)
{
	return this->_channels[itChannel].getLastclientFd();
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
	_clientList.push_back(Client());
	_clientList[_clientList.size() - 1].setPfd(fd, POLLIN);

	std::cout << YELLOW << "New Client Incoming in fd " << _clientList[_clientList.size() - 1].fd() << WHITE << std::endl;

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
	_clientList[0].setPfd(_serverSocket, POLLIN);
	_numberFds++;
}



//---------------------------------------------------//
// Communication Methods
//---------------------------------------------------//

void Server::redirect(int iterator)
{
	_cmd->setInput(_clientList[iterator].sBuffer);
	unsetRevent(iterator);
	_cmd->multiCommands(*this, getClientfd(iterator));
}

/*Fills the buffer with '\0' then recv from pfds[i].
Does not send by itself.*/
int Server::receiveClient(int iterator)
{
	int rv;
	Client client = _clientList[iterator];

	bzero(client.buffer, 1024);
	rv = recv(client.fd(), client.buffer, 1024, 0);
	std::cout << "My buffer: " << client.buffer << std::endl;
	if (rv < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "  recv() failed: " << strerror(errno) << "\n";
			return -1;
		}
		return 0;
	}
	if (rv == 0)
		return -1;
	client.sBuffer.append(client.buffer);
	if (client.sBuffer.find('\n') != std::string::npos)
	{
		redirect(iterator);
		if ((size_t)iterator < _clientList.size() && client.sBuffer.empty() == false)
			client.sBuffer.clear();
	}
	return rv;
}

void Server::sendToChannel(int itChannel, int itClient, std::string message)
{
	if (_channels[itChannel].getSize() > 0)
		_channels[itChannel].sendToChannel(getClientfd(itClient), message);
}

void Server::sendToChannelWithoutPrivateMsg(int it, std::string message)
{
	if (_channels[it].getSize() > 0)
		_channels[it].sendToChannelWithoutPrivateMsg(message);
}


void Server::replyToChannel(int itChannel, int rpl, std::string opt1, std::string opt2)
{
	if (_channels[itChannel].getSize() > 0)
		_channels[itChannel].replyToChannel(*this, rpl, opt1, opt2);
}



//---------------------------------------------------//
// Quitting Client Gestion
//---------------------------------------------------//


/*Closes a clients socket and erase it from _clientList.*/
void Server::closeFd(int itClient)
{
	Client client = _clientList[itClient];
	
	std::cout << YELLOW << "Closing connection with client " << itClient << " its fd is " << getClientfd(itClient) << WHITE << std::endl;

	if (itClient >= 0 && itClient < _numberFds)
	{
		if (client.fd() >= 0)
			close(client.fd());

		if ((size_t)itClient < _clientList.size())
			_clientList.erase(_clientList.begin() + itClient);
	}
}

void Server::deleteUserChannel(int i, int fdClient)
{
	this->_channels[i].deleteUser(fdClient);
}

void Server::addChannelInvitedClient(std::string &name, int i, int fdClient)
{
	if (i == CHANNEL_NOT_EXIST)
	{
		setNewChannel(name);
		i = this->_channels.size() - 1;
	}
	std::string nick = getClientNick(getClientIt(fdClient));
	this->_channels[i].addInvitedClient(nick);
}
