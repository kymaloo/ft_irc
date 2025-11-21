#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

void Command::quit(Server &serv, int it)
{
	// Implementation of the QUIT command
	// TODO PART from all channels

	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// 	serv.printMapChannel(i);

	std::vector<std::string> vecChannel;

	// ? I fill a vector for part
	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.isClientOnChannel(i, serv.getClientIt(it)) == true)
		{
			vecChannel.push_back(serv.getChannelName(i));
		}
	}
	// std::cout << "Je suis le vecChannel dans le fichier QUIT.cpp ligne 23\n";
	// for (size_t i = 0; i != vecChannel.size(); i++)
	// {
	// 	std::cout << vecChannel[i] << std::endl;
	// }
	// ? PART prends le serv, un vector qui contient le nom des channels et le fd
	part(serv, vecChannel, serv.getClientIt(it));
	serv.closeFd(serv.getClientIt(it));
	//serv.compressArray();



	std::cout << "after: QUIT\n";
	for (size_t i = 0; i < serv.getChannelSize(); i++)
		serv.printMapChannel(i);
	return;
}
