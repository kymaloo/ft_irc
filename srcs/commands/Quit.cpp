#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::quit(Server &serv, int it)
{
	// Implementation of the QUIT command
	// TODO PART from all channels


	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.isClientOnChannel(i, it) == true)
			part(serv, i);
	}
	serv.closeFd(it);
	serv.compressArray();
	return;
}
