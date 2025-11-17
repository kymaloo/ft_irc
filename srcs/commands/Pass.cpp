#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void	Command::pass(Server &serv, int it)
{
	if (_params.size() < 1)
	{
		Reply::sendError(serv, 461, it);
		return;
	}
	if (_params[0] != serv.getPass())
	{
		Reply::sendError(serv, 464, it);
		return;
	}
	serv.setClientPass(true, it);

	return;
}
