#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::part(Server &serv, int it)
{
	for (size_t i = 0; i < serv.getChannelSize(); i++)
		serv.printMapChannel(i);

	std::string nick = "Nick : Kymaloo";
	if (checkNumberParam(serv, nick, it) == false)
		return ;

	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);
	
	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		for (size_t j = 0; j != serv.getChannelSize(); j++)
		{
			if (vecChannel[i] == serv.getChannelName(j))
				serv.deleteUserChannel(i, it);
		}
	}
	for (size_t i = 0; i < serv.getChannelSize(); i++)
		serv.printMapChannel(i);
	//serv.printMapChannel(it);
}


// ! Step 1 : Parcourir les channels jusqu'a trouver le channel correspondant

// ! Step 2 : Parcourir la map jusqu'a trouver notre fd 

// ! Step 3 : delete fd de la map