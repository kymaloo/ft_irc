#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::quit(Server &serv, int fdClient)
{
	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);
	clearParams();
	std::cout << "Parting from " << serv.getChannelSize() << " channels\n";
	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.isClientOnChannel(i, fdClient) == true)
		{
			_params.push_back(serv.getChannelName(i));
			
		}
	}
	part(serv, fdClient);
	serv.closeFd(serv.getClientIt(fdClient));
	serv.compressArray();
	return;
}
