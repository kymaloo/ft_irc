#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void	Command::user(Server &serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (_params.empty() || _params.size() < 3)
	{
		Reply::sendError(serv, 461, itClient, "NULL", "NULL");
		return;
	}
	if (serv.getClientUser(itClient).empty())
	{
		serv.setClientUser(_params[0], itClient);
		serv.setClientReal(_params[_params.size() - 1], itClient);
		if (serv.getClientNick(itClient).empty() == false)
		{
			std::cout << GREEN << "Client logged.\n" << WHITE;
			Reply::welcomeClient(serv, itClient);
		}
	}
	else
		Reply::sendError(serv, 462, itClient, "NULL", "NULL");
	return;
}

