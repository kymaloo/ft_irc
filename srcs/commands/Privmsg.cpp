#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

// sendToClient
void sendToClient(Server &serv, std::string &target, std::string message, int it)
{
	for (int i = 0; i < serv.getNumberFds(); i++)
	{
		if (serv.getClientNick(i) == target)
		{
			send(serv.getClientfd(i), message.c_str(), message.size(), 0);
			return;
		}
	}
	Reply::sendError(serv, 401, serv.getClientfd(it), target, "NULL");
	return;
}

// sendToChannel
void sendToChannel(Server &serv, std::string &target, std::string message, int it)
{
	for (size_t i = 0; i < serv.getChannelSize(); i++)
	{
		if (serv.getChannelName(i) == target && serv.isClientOnChannel(i, it) == true)
		{
			serv.sendToChannel(i, message);
			std::cout << "message sent\n";
			return;
		}
	}
	Reply::sendError(serv, 404, it, target, "NULL");

	// serv.printMapChannel(it)
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
	_params[1] += "\r\n";
	sendMessage(serv, targetsVec, _params[1], itClient);

	return;
}
