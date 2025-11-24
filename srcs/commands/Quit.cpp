#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::quit(Server &serv, int fdClient)
{
	// std::vector<std::string> vecChannel;
	// if (!_params[0].empty())
	// 	vecChannel = split(_params[0]);
	clearParams();
	std::cout << "Before: QUIT\n";

	std::string strReturn;

	std::cout << "Parting from " << serv.getChannelSize() << " channels\n";



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

	std::cout << "My params " << _params[0] << std::endl;
	// for (size_t i = 0; i != _params.size(); i++)
	// {
	// 	std::cout << "My params " << _params[i] << std::endl;
	// }
	part(serv, fdClient);
	serv.closeFd(serv.getClientIt(fdClient));
	serv.compressArray();
	std::cout << "After: QUIT\n";
	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// 	serv.printMapChannel(i);
	return;
}
