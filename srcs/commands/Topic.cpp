#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

int checkParams(Server& serv, std::string& command, std::vector<std::string> params, int itClient)
{
	if (params.empty() == true || (params.size() != 1 && params.size() != 2))
	{
		Reply::sendError(serv, 461, itClient, command, "NULL");
		return 0;
	}
	int fdClient = serv.getClientfd(itClient);
	int itChannel = serv.getChannelIterator(params[0]);
	if (serv.doesChannelExist(params[0]) == false || serv.isClientOnChannel(itChannel, fdClient) == false)
	{
		Reply::sendError(serv, 442, itClient, serv.getClientNick(itClient), params[0]);
		return 0;
	}
	if (serv.getChannelMode('t', itChannel) == true && params.size() == 2 && serv.isOpInChannel(itChannel, fdClient) == false)
	{
		Reply::sendError(serv, 482, itClient, params[0], "NULL");
		return 0;
	}
	return params.size();
}

void Command::topic(Server& serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);

	int err = checkParams(serv, _commandName, _params, itClient);
	if (err == 0)
	return;
	
	std::string channelName = _params[0];
	switch (err)
	{
		case 1:
			if (serv.getChannelTopic(_params[0]).empty())
				Reply::sendReply(serv, 331, itClient, channelName, "NULL");
			else
				Reply::sendReply(serv, 332, itClient, channelName, serv.getChannelTopic(channelName));
			return ;
		case 2:
			// _params[1].erase(_params[1].end());
			_params[1].append("\r\n");
			serv.setChannelTopic(channelName, _params[1]);
			serv.replyToChannel(serv.getChannelIterator(channelName), 332, channelName, serv.getChannelTopic(channelName));
			return ;
	}
}
