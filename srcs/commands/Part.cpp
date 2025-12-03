#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::part(Server &serv, int fdClient)
{
	if (checkNumberParam(serv, fdClient) == false)
		return ;
	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		for (size_t j = 0; j != serv.getChannelSize(); j++)
		{
			if (vecChannel[i] == serv.getChannelName(j))
			{
				std::cout << "channel's name: " << serv.getChannelName(j) << std::endl;
				std::cout << "client's fd: " << fdClient << std::endl;
				serv.deleteUserChannel(j, fdClient);
			}
		}
	}
}
