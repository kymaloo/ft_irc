#include "server.hpp"

//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Server::Server()
{
	_numberFds = 0;
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

int Server::setNewClient()
{
	int fd;



	std::cout << "New client : " << _numberFds << ".\n";
	fd = accept(_serverSocket, NULL, NULL);
	if (fd < 0)
	{
		if (errno != EWOULDBLOCK)
			std::cerr << "  accept() failed\n";
		return fd;
	}
	std::cout << "client socket = " << fd << "\n";
	_pfds[_numberFds].fd = fd;
	_pfds[_numberFds].events = POLLIN;
	_numberFds++;
	return fd;
}


//---------------------------------------------------//
// GETTERS

struct pollfd*	Server::getPfds()
{
	return _pfds;
}

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
// OTHER SERVER PROCESS

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

void Server::setUpServer(int port, int n)
{
	int rc, on = 1;
	if (setServSock() < 0)
	{
		std::cerr << "socket() failed\n";
		exit(-1);
	}
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
	rc = ioctl(_serverSocket, FIONBIO, (char *)&on);
	if (rc < 0)
	{
		std::cerr << "ioctl() failed\n";
		close(_serverSocket);
		exit(-1);
	}
	
	//?---------------------------------------

	_pfds[0].fd = _serverSocket;
	_pfds[0].events = POLLIN;
	_numberFds++;
}

int Server::receiveClient(char** buffer, int iterator)
{
	int rv;
	int i = 0;

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
		std::cout << "  Connection closed\n";
		exit(rv);
	}
	while (buffer[i] != '\0')
		i++;
	buffer[0][i] = '\r';
	buffer[0][i] = '\n';
	std::cout << *buffer << "\n";
	// rv = send(_pfds[0].fd, buffer, rv, 0);
	// if (rv < 0)
	// {
	// 	std::cerr << "  send() failed\n";
	// 	exit(rv);
	// }

	sendAll(buffer, iterator);



	return rv;
}

void Server::receiveAll(char** buffer)
{
	int rv;
	for(int i = 0; i < _numberFds && rv > 0; i++)
	{
		rv = receiveClient(buffer, i);
		if (rv < 0)
			break ;
	}
}

void Server::sendAll(char** buffer, int myself)
{
	// std::cout << "Sending packets to " << _numberFds << "fds.\n\n";
	for(int i = 1; i < _numberFds; i++)
	{
		std::cout << "Sending packets to fd " << i << ".\n";
		if (i != myself)
			send(_pfds[i].fd, *buffer, strlen(*buffer), 0);
	}
}
