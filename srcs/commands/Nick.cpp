#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

bool checkDoublon(Server &serv, std::string &cmp)
{
	for (int i = 0; i != serv.getNumberFds(); i++)
	{
		if (cmp == serv.getClientNick(i))
			return (true);
	}
	return (false);
}

bool checkNick(Server &serv, std::vector<std::string> &nickVec, int it)
{
	if (nickVec.empty() || nickVec[0].empty())
	{
		Reply::sendError(serv, 431, it, "NULL", "NULL");
		return false;
	}
	else if (nickVec[0].size() > 15)
	{
		Reply::sendError(serv, 432, it, nickVec[0], "NULL");
		return false;
	}
	else if (checkDoublon(serv, nickVec[0]) == true)
	{
		Reply::sendError(serv, 433, it, nickVec[0], "NULL");
		return false;
	}
	return true;
}

void Command::nick(Server &serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (checkNick(serv, _params, itClient) == false)
		return;
	if (serv.getClientNick(itClient).empty())
	{
		serv.setClientNick(_params[0], itClient);
		if (serv.getClientUser(itClient).empty() == false)
		{
			std::cout << GREEN << "Client logged.\n" << WHITE;
			Reply::welcomeClient(serv, itClient);
		}
	}
	serv.setClientNick(_params[0], itClient);
	return;
}
