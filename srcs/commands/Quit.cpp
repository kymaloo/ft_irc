#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::quit(Server &serv, int fdClient)
{
	clearParams();
	std::string strReturn;
	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.isClientOnChannel(i, fdClient) == true)
		{
			strReturn += serv.getChannelName(i);
			if (i != serv.getChannelSize() - 1)
				strReturn += ",";
		}
	}
	_params.push_back(strReturn);
	part(serv, fdClient);
	serv.closeFd(serv.getClientIt(fdClient));
	return;
}
