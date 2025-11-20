#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void Command::quit(Server &serv, int it)
{
	// Implementation of the QUIT command
	// TODO PART from all channels


	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.isClientOnChannel(i, it) == true)
			part(serv, i);
	}
	for (size_t i = 0; i < serv.getChannelSize(); i++)
		serv.printMapChannel(i);
	serv.closeFd(it);
	serv.compressArray();
	return;
}
