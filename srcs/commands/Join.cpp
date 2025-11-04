#include "../../includes/Command.hpp"

void Command::join(std::string &serverName, std::string &nick, std::string &channel)
{
	std::cout << nick << std::endl;
	std::string message;
    if (nick.empty() || nick[0] != '#')
	{
		message = Reply::ERR_NOSUCHCHANNEL(serverName, nick, channel);
		std::cout << message;
		send(_pfds[1].fd, message.c_str(), message.size(), 0);
		return;
	}
}
