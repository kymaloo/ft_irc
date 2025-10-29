#include "includes/server.hpp"
#include "includes/client.hpp"
#include "includes/Command.hpp"
#include <string.h>

/*Returns true if the whole string is digital.
Does not check if it fits in an int.
Can start with + or -.*/
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

/*Accepts new incoming clients.
Returns true if accept() fails.*/
bool acceptNewClients(Server& serv)
{
	bool end = false;
	int rv = 0;

	while (rv != -1)
	{
		rv = serv.setNewClient();
		if (rv < 0)
			end = true;		
		break ;
	}
	serv.unsetRevent(0);
	return end;
}

/*Tries to receive from a client.
Then tries to send to every client except itself.*/
bool communicate(Server& serv, int it)
{
	char*	buffer = new char[1024];

	if (serv.receiveClient(&buffer, it) == -1 \
			|| serv.sendAll(&buffer, it) == -1)
	{
		serv.closeFd(it);
		return true;
	}
	return false;
}

int main(int argc, char**argv)
{
	bool		end, compress, closeFd = false;
	int			rv, port = -1;
	std::string	sPort = argv[1];
	Server		serv;

	if (argc == 2 || isNum(sPort) == true)
		port = atoi(sPort.c_str());

	// ! TEST PARSE
	std::string input = ":Lucie!lucie@127.0.0.1 JOIN #darkroom";
	Command cmd(input);

	cmd.parse();
	if (cmd.isValid())
	{
		std::cout << "Prefix: " << cmd.getPrefix() << std::endl;
		std::cout << "Commande: " << cmd.getName() << std::endl;

		std::cout << "Params:" << std::endl;
		for (size_t i = 0; i < cmd.getParams().size(); ++i)
			std::cout << "  [" << i << "]: " << cmd.getParams()[i] << std::endl;
	}

	serv.setUpServer(port, 5);
	do
	{
		compress = false;
		closeFd = false;

		rv = poll(serv.getPfds(), serv.getNumberFds(), -1);
		if (rv < 0)
		{
			std::cerr <<"  poll() failed\n";
			break;
		}
		for (int i = 0; i < serv.getNumberFds(); i++)
		{

			if (serv.getRevents(i) == 0)
				continue ;
			if (serv.getRevents(i) != POLLIN)
				break ;
			if (i == 0)
				end = acceptNewClients(serv);
			else
				compress = communicate(serv, i);
		}
		if(compress == true)
			serv.compressArray();
	} while (end == false);

    return 0;
}
