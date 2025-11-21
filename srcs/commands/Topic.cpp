#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

int checkParams(Server& serv, std::string& command, std::vector<std::string> params, int itClient)
{
	if (params.empty() == true || (params.size() != 1 && params.size() != 2))
	{
		Reply::sendError(serv, 461, itClient, command, "NULL");
		return 0;
	}
	else if (serv.doesChannelExist(params[0]) == false || serv.isClientOnChannel(serv.getChannelIterator(params[0]), serv.getClientfd(itClient)) == false)
	{
		Reply::sendError(serv, 442, itClient, serv.getClientNick(itClient), params[0]);
		return 0;
	}	
	return params.size();
}

void Command::topic(Server& serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);

	int err = checkParams(serv, _commandName, _params, itClient);
	switch (err)
	{
		case 0:
			return ;
		case 1:
			if (serv.getChannelTopic(_params[0]).empty())
				Reply::sendReply(serv, 331, itClient, _params[0], "NULL");
			else
				Reply::sendReply(serv, 332, itClient, _params[0], serv.getChannelTopic(_params[0]));
			return ;
		case 2:
			serv.setChannelTopic(_params[0], _params[1]);
	}
}
