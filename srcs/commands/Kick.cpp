#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

void Command::kick(Server &serv, int fdClient)
{
	if (checkChannelforKick(serv, fdClient) == false)
		return ;
	if (checkUserForKick(serv, fdClient) == false)
		return ;
}

bool Command::checkChannelforKick(Server &serv, int fdClient)
{
	if (checkNumberParam(serv, fdClient) == false)
		return false;
	int itClient = serv.getClientIt(fdClient);
	std::vector<std::string> vecChannel;
	if (!_params[0].empty())
		vecChannel = split(_params[0]);

	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		if (serv.isClientOnChannel(i, fdClient) == false)
		{
			Reply::sendError(serv, 441, itClient, serv.getClientNick(itClient), vecChannel[i]);
			return false;
		}
		if (serv.isOpInChannel(i, fdClient) == false)
		{
			Reply::sendError(serv, 482, itClient, serv.getClientNick(itClient), vecChannel[i]);
			return false;
		}
	}
	return true;
}

bool Command::checkUserForKick(Server &serv, int fdClient)
{
	if (checkNumberParam(serv, fdClient) == false)
		return false;
	std::vector<std::string>	vecUser;
	std::string 				comment;

	if (_params.size() > 0 && !_params[1].empty())
		vecUser = split(_params[1]);
	else
		return false;
	if (_params.size() > 2)
	{
		comment = _params[2];
		while (comment[comment.size() - 1] == '\r' || comment[comment.size() - 1] == '\n')
				comment.erase(comment.size() - 1);
		comment.append("\r\n");
	}
	for (size_t i = 0; i != vecUser.size(); i++)
	{
		if (serv.getClientfd(vecUser[i]) == -1)
			continue;
		part(serv, serv.getClientfd(vecUser[i]));
		if (comment.empty() == false)
			send(serv.getClientfd(vecUser[i]), comment.c_str(), comment.size(), 0);
	}
	return true;
}
