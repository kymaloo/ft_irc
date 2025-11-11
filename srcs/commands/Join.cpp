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

void Command::join(Server &serv, std::string &nick, int it)
{
	for (size_t i = 0; i != _params.size(); i++)
	{
		if (isNameChannelValid(serv, nick, _params[i], it) == false)
		{
			std::cout << "It's me\n";
			return ;
		}
		serv._channels.push_back(Channel(_params[i], it));
	}
	for (size_t i = 0; i < serv._channels.size(); i++)
	{
		std::cout << serv._channels[i].getName() << std::endl;
	}
}
