#include "server/server.hpp"
#include "client/client.hpp"

bool	isNum(std::string str)
{
	int i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (std::isdigit(str[i]) == false)
			return false;
		i++;
	}
	return true;
}

int main(int argc, char**argv)
{

	std::string sPort = argv[1];
	if (argc != 2 || isNum(sPort) == false)
		return 1;
	int port = atoi(sPort.c_str());

	Server serv;

    // creating socket
    serv.setServSock();
    // specifying the address
	serv.setSockAddr(port);
    // binding socket.
    serv.bindSock();
    // listening to the assigned socket
    serv.listenClient(5);


    // accepting connection request

	Client cl;

	cl.newClient(serv.getServSock());

    char* buffer = new char[1024];
	cl.receiveAll(&buffer);
	//!tout les clients ecrivent dans le meme buffer pour l'instant


    // recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;

    // closing the socket.
    // close(serverSocket);

    return 0;
}
