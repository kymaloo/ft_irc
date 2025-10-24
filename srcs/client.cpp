#include "../includes/client.hpp"


//---------------------------------------------------//
// CONSTRUCTOR/DESTRUCTOR

Client::Client()
{
	_fds = new int[1];
	_fds[0] = 0;
}

Client::~Client()
{
	delete []_fds;
}


//---------------------------------------------------//
// SETTERS

/*Links a client to the socket to establish communication.*/
void	Client::setClientSock(int serverSocket, int iterator)
{
	this->_clients[iterator] = accept(serverSocket, NULL, NULL);
}


//---------------------------------------------------//
// GETTERS

int*	Client::getClientList()
{
	return _fds;
}

int	Client::getSize()
{
	return _clients.size();
}

int		Client::getClientFd(int iterator)
{
	return _clients[iterator];
}


//---------------------------------------------------//
// OTHER CLIENT PROCESS

/**/
void Client::receive(char** buffer, int iterator)
{
	recv(_clients[iterator], *buffer, sizeof(*buffer), 0);
}

void Client::receiveAll(char** buffer)
{
	for(size_t i = 0; i < _clients.size(); i++)
		receive(buffer, i);
}

void	Client::newClient(int serverSocket)
{
	delete []_fds;
	_clients.push_back(accept(serverSocket, NULL, NULL));
	_fds = new int[_clients.size()];
	//TODO : rajouter les anciens bidules dedans
	_fds[_clients.size() - 1] = _clients[_clients.size() - 1];
}
