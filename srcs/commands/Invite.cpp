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
	
	itClient = serv.getClientIt(serv.getClientfd(clientName));
	if (checkNumberParam(serv, itClient) == false)
		return ;
	if (isParamsGood(_params) == false)
		return;
	clientName = _params[0];
	channelName = _params[1];
	if (serv.getClientNick(serv.getClientIt(fdClient)) == "")
	{
		Reply:: ERR_NOSUCHNICK(serv.getServName(), clientName, channelName);
		return ;
	}
	if (serv.isClientOnChannel(serv.getChannelIterator(channelName), fdClient) == false)
	{
		Reply::ERR_NOTONCHANNEL(serv.getServName(), serv.getClientNick(serv.getClientIt(fdClient)), channelName);
		return ;
	}
	else
	{
		Reply::ERR_USERONCHANNEL(serv.getServName(), serv.getClientNick(serv.getClientIt(fdClient)), channelName);
		return ;
	}
	if (serv.isOpInChannel(serv.getClientIt(fdClient), fdClient) == false)
	{
		Reply::sendError(serv, 482, serv.getClientIt(fdClient), channelName, "NULL");
		return ;
	}
	if (isNameChannelValid(serv, channelName, serv.getClientIt(fdClient)) == false)
		return ;
	if (isChannelIntoList(serv, channelName) == false)
		serv.addChannelInvitedClient(channelName, CHANNEL_NOT_EXIST, serv.getClientfd(clientName));
	serv.addChannelInvitedClient(channelName, serv.getChannelSize() - 1, serv.getClientfd(clientName));
	serv.setChannelMode('i', true, serv.getChannelSize() - 1, "NULL");
	Reply::RPL_INVITING(serv.getServName(), channelName, clientName);
}
