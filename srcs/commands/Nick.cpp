#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void	Command::nick(Server &serv, int it)
{
	// TODO changer la longeur max pour que le NICK soit de 15 char max, pas la ligne entière
	if (_params[0].size() > 15)
	{
		Reply::sendError(serv, 432, it);
		return;
	}

	// send welcome only if both nick and user are set for the first time
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