#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

// sendToClient
std::string sendToClient(Server &serv, std::string &target, std::string message)
{
	for (int i = 0; i < serv.getNumberFds(); i++)
	{
		if (serv.getClientNick(i) == target)
		{
			send(serv.getClientfd(i), message.c_str(), message.size(), 0);
			break ;
		}
	}
	return "client";
}

// sendToChannel
std::string sendToChannel()
{
	std::cout << "Sending to channel" << std::endl;
	return "channel";
}

// sendMessage
std::string sendMessage(Server &serv, std::vector<std::string> targets, std::string message)
{
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i][0] == '#')
			sendToChannel();
		else
			sendToClient(serv, targets[i], message);
	}
	return message;
}

std::string Command::privmsg(Server &serv, int it)
{
	std::vector<std::string> targetsVec;

	// Checking for enough parameters
	if (_params.size() < 2)
	{
		Reply::sendError(serv, 461, it);
		return "ERROR";
	}
	targetsVec = split(_params[0]);
	_params[1] += "\r\n";
	sendMessage(serv, targetsVec, _params[1]);

	return _params[1];
}
