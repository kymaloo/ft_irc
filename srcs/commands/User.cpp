#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void	Command::user(Server &serv, int it)
{
	if (_params.empty() || _params.size() < 3)
	{
		Reply::sendError(serv, 461, it, "NULL", "NULL");
		return;
	}
	if (serv.getClientUser(it).empty())
	{
		serv.setClientUser(_params[0], it);
		serv.setClientReal(_params[_params.size() - 1], it);
		if (!serv.getClientNick(it).empty())
		{
			std::cout << GREEN << "Client logged.\n" << WHITE;
			Reply::welcomeClient(serv, it);
		}
	}
	else
		Reply::sendError(serv, 462, it, "NULL", "NULL");
	return;
}

