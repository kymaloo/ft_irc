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

void Command::join(Server &serv, std::string &nick, std::string &channel, int it)
{
	//std::cout << channel << std::endl;
	//std::cout << "Params[0] : " << _params[0] << std::endl;
	for(size_t i = 0; i != _params.size(); i++)
		std::cout << _params[i] << std::endl;
	if (isNameChannelValid(serv, nick, channel, it) == false)
	{
		std::cout << "It's me\n";
		return ;
	}
// 	serv._channels.push_back(Channel(channel, it));
// 	for (size_t i = 0; i < serv._channels.size(); i++)
// 	{
// 		std::cout << serv._channels[0].getName() << std::endl;
// 	}
}
