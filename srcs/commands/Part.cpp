#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::part(Server &serv, int fdClient)
{
	if (checkNumberParam(serv, fdClient) == false)
		return ;
	bool inChannel = false;
	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		if (isNameChannelValid(serv, vecChannel[i], serv.getClientIt(fdClient)) == false)
		{
			Reply:: ERR_NOSUCHCHANNEL(serv.getServName(), serv.getClientNick(serv.getClientIt(fdClient)), vecChannel[i]);
			continue;
		}
		for (size_t j = 0; j != serv.getChannelSize(); j++)
		{
			if (vecChannel[i] == serv.getChannelName(j))
			{
				std::cout << "It's my name: " << serv.getChannelName(j) << std::endl;
				std::string msg;
				msg += ":" + serv.getClientNick(serv.getClientIt(fdClient)) + "!" + serv.getClientUser(serv.getClientIt(fdClient)) + " PART " + vecChannel[i] + "\r\n";
				serv.sendToChannelWithoutPrivateMsg(serv.getChannelIterator(vecChannel[i]), msg);
				serv.deleteUserChannel(j, fdClient);
				inChannel = true;
			}
		}
		if (inChannel == false)
			Reply::ERR_NOTONCHANNEL(serv.getServName(), serv.getClientNick(serv.getClientIt(fdClient)), vecChannel[i]);
	}
}
