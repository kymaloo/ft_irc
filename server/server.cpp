#include "server.hpp"

//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Server::Server()
{

}

Server::~Server()
{
    close(_serverSocket);
}


//---------------------------------------------------//
// SETTERS

/*Creates and init a new TCP socket.
AF_INET for ipv4, SOCK_STREAM for TCP, 0 for protocol auto.*/
void Server::setServSock()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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
void Server::bindSock()
{
    bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
}

/*Prepare to accept connections on socket FD.
   [n] connection requests will be queued before further requests are refused.*/
void Server::listenClient(int n)
{
	listen(_serverSocket, n);
}

