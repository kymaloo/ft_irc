#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

static bool isParamsGood(std::vector<std::string> vec)
{
	std::vector<std::string> vecChannelName;
	std::vector<std::string> vecClientName;

	if (!vec[0].empty())
		vecChannelName = split(vec[0]);
	if (vecChannelName.size() != 1)
		return false;
	if (!vec[1].empty())
		vecClientName = split(vec[1]);
	if (vecClientName.size() != 1)
		return false;
	return true;
}

void Command::invite(Server &serv, int fdClient)
{
	int			itClient;
	std::string	channelName;
	std::string	clientName;
	
	if (isParamsGood(_params) == false)
		return;
	clientName = _params[0];
	channelName = _params[1];

	itClient = serv.getClientIt(serv.getClientfd(clientName));
	if (isNameChannelValid(serv, channelName, serv.getClientIt(fdClient)) == false)
		return ;
	if (isChannelIntoList(serv, channelName) == false)
		serv.addChannelInvitedClient(channelName, CHANNEL_NOT_EXIST, serv.getClientfd(clientName));
	serv.addChannelInvitedClient(channelName, serv.getChannelSize() - 1, serv.getClientfd(clientName));
	serv.setChannelMode('i', true, serv.getChannelSize() - 1, "NULL");
}
