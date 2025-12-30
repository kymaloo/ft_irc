#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

// sendToClient
void sendToClient(Server &serv, std::string &target, std::string message, int it)
{
	for (int i = 0; i < serv.getNumberFds(); i++)
	{
		if (serv.getClientNick(i) == target)
		{
			message = Reply::RPL_PRIVMSG(serv.getClientNick(it), target, message);
			send(serv.getClientfd(i), message.c_str(), message.size(), 0);
			return;
		}
	}
	Reply::sendError(serv, 401, it, target, "NULL");
	return;
}

// sendToChannel
void sendToChannel(Server &serv, std::string &target, std::string message, int it)
{
	for (size_t i = 0; i < serv.getChannelSize(); i++)
	{
		if (serv.getChannelName(i) == target)
		{
			message = Reply::RPL_PRIVMSG(serv.getClientNick(it), serv.getChannelName(i), message);
			if (serv.isClientOnChannel(i, serv.getClientfd(it)) == true)
				serv.sendToChannel(i, it, message);
			else
				Reply::sendError(serv, 404, it, target, "NULL");
			return;
		}
	}
	Reply::sendError(serv, 403, it, target, "NULL");
	return;
}

// sendMessage
void sendMessage(Server &serv, std::vector<std::string> targets, std::string message, int it)
{
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i][0] == '#')
			sendToChannel(serv, targets[i], message, it);
		else
			sendToClient(serv, targets[i], message, it);
	}
	return;
}

void Command::privmsg(Server &serv, int fdClient)
{
	std::vector<std::string> targetsVec;
	int itClient = serv.getClientIt(fdClient);

	// Checking for enough parameters
	if (_params.empty() || _params.size() < 2)
	{
		Reply::sendError(serv, 412, itClient, "NULL", "NULL");
		return;
	}
	targetsVec = split(_params[0]);
	std::string message = _params[1];

	while (message[message.size() - 1] == '\r' || message[message.size() - 1] == '\n')
		message.erase(message.size() - 1);
	sendMessage(serv, targetsVec, message, itClient);
	return;
}
