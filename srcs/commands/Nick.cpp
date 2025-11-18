#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

bool checkDoublon(Server &serv, std::string &cmp)
{
	for (int i = 0; i != serv.getNumberFds(); i++)
	{
		if (cmp == serv.getClientNick(i))
			return (true);
	}
	return (false);
}

bool checkNick(Server &serv, std::string &nick, int it)
{
	if (nick.empty())
	{
		Reply::sendError(serv, 431, it, "NULL", "NULL");
		return false;
	}
	else if (nick.size() > 15)
	{
		Reply::sendError(serv, 432, it, nick, "NULL");
		return false;
	}
	else if (checkDoublon(serv, nick) == true)
	{
		Reply::sendError(serv, 433, it, nick, "NULL");
		return false;
	}
	return true;
}

void	Command::nick(Server &serv, int it)
{
	if (checkNick(serv, _params[0], it) == false)
		return;
	if (serv.getClientNick(it).empty())
	{
		serv.setClientNick(_params[0], it);
		if (serv.getClientUser(it).empty() == false)
		{
			std::cout << GREEN << "Client logged.\n" << WHITE;
			Reply::welcomeClient(serv, it);
		}
	}
	serv.setClientNick(_params[0], it);
	std::cout << "Nick set to : " << serv.getClientNick(it) << std::endl;
	return;
}
