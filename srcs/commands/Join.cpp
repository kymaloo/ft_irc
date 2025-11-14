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


void Command::join(Server &serv, std::string &nick, int it)
{
	if (checkNumberParam(serv, nick, it) == false)
		return ;
	
	// std::vector<std::string> vecChannel;
	// if (!_params[0].empty())
	// 	vecChannel = split(_params[0]);
	// for (size_t i = 0; i != vecChannel.size(); i++)
	// {
	// 	if (isNameChannelValid(serv, nick, vecChannel[i], it) == true)
	// 		serv._channels.push_back(Channel(vecChannel[i], it));
	// }
	// for (size_t i = 0; i < serv._channels.size(); i++)
	// {
	// 	std::cout << serv._channels[i].getName() << std::endl;
	// }
}
