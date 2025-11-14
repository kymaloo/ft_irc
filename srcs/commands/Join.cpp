#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

bool Command::isNameChannelValid(Server &serv, std::string &nick, std::string &channel, int it)
{
	//std::cout << nick << std::endl;
	std::string message;
	pollfd *pfds = serv.getPfds();
    if (channel.empty() || channel[0] != '#')
	{
		message = Reply::ERR_NOSUCHCHANNEL(serv.getServName(), nick, channel);
		std::cerr << message << std::endl;
		send(pfds[it].fd, message.c_str(), message.size(), 0);
		return (false);
	}
	return (true);
}

bool Command::checkNumberParam(Server &serv, std::string &nick, int it)
{
	std::string message;
	pollfd *pfds = serv.getPfds();
	if (_params[0].empty())
	{
		message = Reply::ERR_NEEDMOREPARAMS(serv.getServName(), nick, _commandName);
		std::cerr << message << std::endl;
		send(pfds[it].fd, message.c_str(), message.size(), 0);
		return (false);
	}
	return (true);
}

// bool Command::isChannelInToList(Server &serv, std::vector<std::string> &vecChannel, std::string &nick, int it)
// {
// 	for (size_t i = 0; i != serv._channels.size(); i++)
// 	{
// 		if (serv._channels[i].getName() == vecChannel[it])
// 			return (true);
// 	}
// 	return (false);
// }

void Command::checkEntryChannel(Server &serv, std::string &nick, int it)
{
	std::vector<std::string> vecChannel;
	std::vector<std::string> vecMdp;

	if (!_params[0].empty())
		vecChannel = split(_params[0]);
	if (!_params[1].empty())
		vecMdp = split(_params[1]);



	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		if (isNameChannelValid(serv, nick, vecChannel[i], it) == false)
			continue;
		// if (isChannelInToList(serv, vecChannel, nick, it) == true && serv._channels[it].getIsPassWorld() == true)
		// 	continue;
		// 	serv._channels.push_back(Channel(vecChannel[i], it));
	
	}
	// for (size_t i = 0; i < serv._channels.size(); i++)
	// {
	// 	std::cout << serv._channels[i].getName() << std::endl;
	// }
}

// ! Step 1: Verifier si le channel est good
// ! Step 2: Verifier s'il y a un mdp
// ! Step 3: Verifier si les mdp corresponde




void Command::join(Server &serv, std::string &nick, int it)
{
	if (checkNumberParam(serv, nick, it) == false)
		return ;
	
}
