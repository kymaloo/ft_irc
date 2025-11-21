#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void	Command::pass(Server &serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (_params.empty())
	{
		Reply::sendError(serv, 461, itClient, "NULL", "NULL");
		return;
	}
	if (_params[0] != serv.getPass())
	{
		Reply::sendError(serv, 464, itClient, "NULL", "NULL");
		return;
	}
	serv.setClientPass(true, itClient);

	return;
}
