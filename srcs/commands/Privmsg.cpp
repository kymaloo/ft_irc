#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

// sendToUser
std::string sendToClient(Server &serv, std::string &nick, std::string message)
{
	(void)serv;
	(void)nick;
	(void)message;
	// send(serv)
	return "client";
}

// sendToChannel
std::string sendToChannel()
{
	return "channel";
}

// sendMessage
std::string sendMessage(Server &serv, std::string &nick, std::vector<std::string> targets, std::string message)
{
	// std::vector<int> sended;
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i][0] == '#')
			sendToChannel();
		else
			sendToClient(serv, nick, message);
	}
	return "Gloubiboulga :)";
}

std::string Command::privmsg(Server &serv, std::string &nick, std::string line, int it)
{
	std::vector<std::string> targetsVec;
	std::string tmp;
	size_t		prevPos = line.find(" ");
	size_t		pos = line.find(" ");

	(void)nick;
	// print things
	std::cout << "input : " << _input << std::endl;
	std::cout << "command : " << _commandName << std::endl;
	if (_prefix.empty() == false)
		std::cout << "prefix : " << _prefix << std::endl;
	for (size_t i = 0; i < _params.size(); i++)
		std::cout << "params : " << _params[i] << std::endl;
	// Checking for enough parameters
	if (line.length() <= 8)
	{
		serv.sendError(461, it);
		return "ERROR";
	}

	// Getting targets
	tmp = line.substr(prevPos + 1, pos - prevPos - 1);
	std::cout << "tmp line : " << tmp << std::endl;

	for (size_t comma = 0; comma != std::string::npos; comma = tmp.find(",", prevPos))
	{
		targetsVec.push_back(tmp.substr(prevPos, comma - prevPos));
		prevPos = comma + 1;
	}
	targetsVec.push_back(tmp.substr(prevPos, pos - prevPos));

	// Displaying targets
	for (size_t i = 0; i < targetsVec.size(); i++)
		std::cout << "Target " << i << " : " << targetsVec[i] << std::endl;

	// Getting message
	pos = line.find(" :", pos);
	if (pos == std::string::npos)
	{
		serv.sendError(412, it);
		return "ERROR";
	}
	tmp = line.substr(pos + 2, line.length() - pos - 3);

	std::cout << "Message : " << tmp << std::endl;

	// TODO envoyer le message aux targets

	return tmp;

}