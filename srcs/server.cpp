#include "../includes/server.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR
//---------------------------------------------------//


Server::Server()
{
	_numberFds = 0;
	_serverName = "Default";
}

Server::Server(std::string& name)
{
	_numberFds = 0;
	_serverName = name;
}

Server::~Server()
{
    close(_serverSocket);
	for (int i = 0; i < 199; i++)
	{
		if(_pfds[i].fd >= 0)
		close(_pfds[i].fd);
	}
}



//---------------------------------------------------//
// SETTERS
//---------------------------------------------------//




/*Tries to accept a new client.
Sets the client address up.
Returns its fd.*/
int Server::setNewClient()
{
	int fd;


	fd = accept(_serverSocket, NULL, NULL);
	if (fd < 0)
	{
		if (errno != EWOULDBLOCK)
			std::cerr << "  accept() failed\n";
		return fd;
	}
	std::cout << GREEN << "New Client Connection.\n" << WHITE;

	_pfds[_numberFds].fd = fd;
	_pfds[_numberFds].events = POLLIN;
	_numberFds++;
	return fd;
}

/*set pfds[i].revents to 0.*/
void Server::unsetRevent(int i)
{
	this->_pfds[i].revents = 0;
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







//---------------------------------------------------//
// Setup Methods
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


	rv = recv(_pfds[iterator].fd, *buffer, sizeof(*buffer), 0);
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
	while (buffer[i] != '\0')
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
			rv = send(_pfds[i].fd, *buffer, strlen(*buffer), 0);
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
