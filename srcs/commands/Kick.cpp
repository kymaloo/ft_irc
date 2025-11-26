#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::kick(Server &serv, int fdClient)
{
	if (checkNumberParam(serv, fdClient) == false)
		return ;

	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		// if (isNameChannelValid(serv, vecChannel[i], serv.getClientIt(fdClient)) == false)

		//if (serv.isClientOnChannel(i, fdClient) == false)
		// ! check send error + return
			//Reply::sendError(serv, 442, serv.getClientIt(fdClient), vecChannel[i], "");
		//if (serv.isOpInChannel(i, fdClient) == false)
		// ! check send error + return
			//Reply::sendError(serv, 482, serv.getClientNick(serv.getClientIt(fdClient)))
	}


}

