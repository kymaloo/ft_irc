#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

int checkParams(Server& serv, std::vector<std::string> params, int it)
{
	if (params.size() != 1 && params.size() != 2)
		Reply::sendError(serv, 461, it, NULL, NULL);
	else if (serv.doesChannelExist(params[0]) == false || serv.isClientOnChannel(serv.getChannelIterator(params[0]), it) == false)
		Reply::sendError(serv, 442, it, serv.getClientNick(it), params[0]);
	return params.size();
}

void Command::topic(Server& serv, int it)
{
	if (checkParams(serv, _params, it) == 1)
		Reply::sendReply();
}

        //    ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
        //    RPL_NOTOPIC                     RPL_TOPIC