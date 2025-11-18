#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::part(Server &serv, int it)
{
	std::string nick = "Nick : Kymaloo";
	if (checkNumberParam(serv, nick, it) == false)
		return ;

	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);
	
	
	//std::vector<Channel> tmp = serv.getChannel();
	

	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		size_t j = 0;
		for (std::vector<Channel>::iterator chanIt = serv._channels.begin(); chanIt != serv._channels.end(); ++chanIt)
		{
    		//std::map<int, bool> fdMap = chanIt->_fdClient;
    		for (std::map<int, bool>::iterator it = chanIt->_fdClient.begin(); it != chanIt->_fdClient.end(); ++it)
    		{
				if (vecChannel[i] == serv._channels[j].getName())
					serv._channels.erase(serv._channels.begin() + j);
				j++;
					
    		}
		}
		
	}
	//serv.setChannel(tmp);
	serv._channels[0].printMap();
}
