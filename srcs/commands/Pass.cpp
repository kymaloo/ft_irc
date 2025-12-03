#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void	Command::pass(Server &serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (_params.empty())
		return Reply::sendError(serv, 461, itClient, "NULL", "NULL");
	if (_params[0] != serv.getPass())
		return Reply::sendError(serv, 464, itClient, "NULL", "NULL");
	if (serv.didClientPass(itClient) == true)
		return Reply::sendError(serv, 462, itClient, "NULL", "NULL");

	return serv.setClientPass(true, itClient);
}
