#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void Command::quit(Server &serv, int it)
{
	// Implementation of the QUIT command
	// TODO PART from all channels


	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		part(serv, it);
	}
	serv.closeFd(it);
	serv.compressArray();
	return;
}
