#include "server.hpp"

//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Server::Server()
{

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


//---------------------------------------------------//
// GETTERS

struct pollfd*	Server::getPfds()
{
	return _pfds;
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
		perror("socket() failed");
		exit(-1);
	}
	rc = setsockopt(_serverSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		exit(-1);
	}
	setSockAddr(port);
	if (bindSock() < 0)
	{
		perror("bind() failed");
		exit(-1);
	}
	if (listenClient(n) < 0)
	{
		perror("listen() failed");
		exit(-1);
	}

	//?---------------------------------------

	_pfds[0].fd = _serverSocket;
	_pfds[0].events = POLLIN;
}
