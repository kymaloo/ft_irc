#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

int checkParams(Server& serv, std::string& command, std::vector<std::string> params, int it)
{
	if (params.empty() == true || (params.size() != 1 && params.size() != 2))
	{
		Reply::sendError(serv, 461, it, command, "NULL");
		return 0;
	}
	else if (serv.doesChannelExist(params[0]) == false || serv.isClientOnChannel(serv.getChannelIterator(params[0]), it) == false)
	{
		Reply::sendError(serv, 442, it, serv.getClientNick(it), params[0]);
		return 0;
	}	
	return params.size();
}

void Command::topic(Server& serv, int it)
{
	int err = checkParams(serv, _commandName, _params, it);
	switch (err)
	{
		case 0:
			return ;
		case 1:
			if (serv.getChannelTopic(_params[0]).empty())
				Reply::sendReply(serv, 331, it, _params[0], "NULL");
			else
				Reply::sendReply(serv, 332, it, _params[0], serv.getChannelTopic(_params[0]));
			return ;
		case 2:
			serv.setChannelTopic(_params[0], _params[1]);
	}
}
