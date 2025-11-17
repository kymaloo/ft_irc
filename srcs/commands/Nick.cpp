#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

std::string	Command::nick(Server &serv, int iterator)
{
	// TODO changer la longeur max pour que le NICK soit de 15 char max, pas la ligne entière
	if (_params[1].size() > 15)
	{
		Reply::sendError(serv, 432, iterator);
		return "ERROR";
	}

	// send welcome only if both nick and user are set for the first time
	if (serv.getClientNick(iterator).empty())
	{
		serv.setClientNick(_params[1], iterator);
		// serv.setClientNick(line.substr(5, line.length() - 5), iterator);
		if (serv.getClientNick(iterator).empty() == false)
		{
			std::cout << GREEN << "Client logged.\n" << WHITE;
			Reply::welcomeClient(serv, iterator);
		}
	}
	serv.setClientNick(_params[1], iterator);
	// serv.setClientNick(line.substr(5, line.length() - 5), iterator);	
	std::cout << "Nick set to : " << serv.getClientNick(iterator) << std::endl;
	return serv.getClientNick(iterator);
}