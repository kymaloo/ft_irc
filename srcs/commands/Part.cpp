#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

// void Command::part(Server &serv, int fdClient)
// {
// 	int itClient = serv.getClientIt(fdClient);
// 	for (size_t i = 0; i < serv.getChannelSize(); i++)
// 		serv.printMapChannel(i);

// 	if (checkNumberParam(serv, itClient) == false)
// 		return ;

// 	std::vector<std::string> vecChannel;
// 	if (!_params[0].empty())
// 		vecChannel = split(_params[0]);
	
// 	for (size_t i = 0; i != vecChannel.size(); i++)
// 	{
// 		for (size_t j = 0; j != serv.getChannelSize(); j++)
// 		{
// 			if (vecChannel[i] == serv.getChannelName(j))
// 			{
// 				std::cout << YELLOW << "Parting from " << serv.getChannelName(i) << WHITE << std::endl;				
// 				serv.deleteUserChannel(i, fdClient);
// 			}
// 		}
// 	}
// 	std::cout << "\n\n channel list :\n";
// 	for (size_t i = 0; i < serv.getChannelSize(); i++)
// 		serv.printMapChannel(i);
// }


// ! Step 1 : Parcourir les channels jusqu'a trouver le channel correspondant

// ! Step 2 : Parcourir la map jusqu'a trouver notre fd 

// ! Step 3 : delete fd de la map


void Command::part(Server &serv, int fdClient)
{
	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// 	serv.printMapChannel(i);

	if (checkNumberParam(serv, fdClient) == false)
		return ;

	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	// std::cout << "In PART\n";
	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		for (size_t j = 0; j != serv.getChannelSize(); j++)
		{
			if (vecChannel[i] == serv.getChannelName(j))
			{
				std::cout << "It's my name: " << serv.getChannelName(j) << std::endl;
				serv.deleteUserChannel(j, fdClient);
			}
		}
	}
	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// 	serv.printMapChannel(i);
}