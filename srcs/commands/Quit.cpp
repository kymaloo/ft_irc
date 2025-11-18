#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void Command::quit(Server &serv, int it)
{
	// Implementation of the QUIT command
	// TODO PART from all channels
	serv.closeFd(it);
	serv.compressArray();
	return;
}
