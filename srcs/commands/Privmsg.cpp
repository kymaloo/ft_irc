#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

// sendToClient
std::string sendToClient(Server &serv, std::string &target, std::string message)
{
	for (int i = 0; i < serv.getNumberFds(); i++)
	{
		if (serv.getClientNick(i) == target)
		{
			std::cout << "Sending to client " << target << std::endl;
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
std::string sendMessage(Server &serv, std::string &nick, std::vector<std::string> targets, std::string message)
{
	// std::vector<int> sended;
	(void)nick;
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i][0] == '#')
			sendToChannel();
		else
			sendToClient(serv, targets[i], message);
	}
	return message;
}

std::string Command::privmsg(Server &serv, std::string &nick, std::string line, int it)
{
	std::vector<std::string> targetsVec;

	(void)nick;
	// Checking for enough parameters
	if (line.length() <= 8)
	{
		Reply::sendError(serv, 461, it);
		return "ERROR";
	}

	targetsVec = split(_params[0]);
	// Displaying targets
	for (size_t i = 0; i < targetsVec.size(); i++)
		std::cout << "Target " << i << " : " << targetsVec[i] << std::endl;

	std::cout << "Message : " << _params[1] << std::endl;

	sendMessage(serv, nick, targetsVec, _params[1]);
	// TODO envoyer le message aux targets

	return _params[1];

}
