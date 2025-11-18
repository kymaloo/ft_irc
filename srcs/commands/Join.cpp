#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

bool Command::isNameChannelValid(Server &serv, std::string &nick, std::string &channel, int it)
{
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

bool Command::isMdpValid(Server &serv, std::string &channel, int it)
{
	std::string message;
	pollfd *pfds = serv.getPfds();
    if (channel.empty() || channel[0] != '#')
	{
		message = Reply::ERR_BADCHANNELKEY(serv.getServName(), channel);
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

bool Command::isChannelIntoList(Server &serv, std::string &vecChannel)
{
	for (size_t i = 0; i != serv._channels.size(); i++)
	{
		if (serv._channels[i].getName() == vecChannel)
			return (true);
	}
	return (false);
}

size_t Command::getIteratorChannel(Server &serv, std::string &vecChannel)
{
	for (size_t i = 0; i != serv._channels.size(); i++)
	{
		if (serv._channels[i].getName() == vecChannel)
			return (i);
	}
	return (0);
}

void Command::checkEntryChannel(Server &serv, std::string &nick, int user)
{
	std::vector<std::string> vecChannel;
	std::vector<std::string> vecMdp;
	size_t j = 0;

	if (!_params[0].empty())
		vecChannel = split(_params[0]);
	if (_params.size() > 1 && !_params[1].empty())
		vecMdp = split(_params[1]);

	for (size_t i = 0; i != vecChannel.size(); i++)
	{
		if (isNameChannelValid(serv, nick, vecChannel[i], user) == false)
		{
			if (j != vecMdp.size())
				j++;
			continue;
		}
		if (_params.size() > 1 && !_params[1].empty())
		{
			if (isChannelIntoList(serv, vecChannel[i]) == true && serv._channels[getIteratorChannel(serv, vecChannel[i])].isPassWorld() == true)
			{
				if (serv._channels[getIteratorChannel(serv, vecChannel[i])].getPassWorld() == vecMdp[j])
					serv._channels[getIteratorChannel(serv, vecChannel[i])].addClient(user);
			}
			else
			{
				isMdpValid(serv, vecChannel[i], user);
			}
			if (j != vecMdp.size())
				j++;
		}
		else
		{
			if (isChannelIntoList(serv, vecChannel[i]) == true )
			{
				serv._channels[getIteratorChannel(serv, vecChannel[i])].addClient(user);
			}
			if (isChannelIntoList(serv, vecChannel[i]) == false)
			{
				serv._channels.push_back(Channel(vecChannel[i], user, true));

			}
		}
	}
	std::cout << "C'est moi qui print grosse merde\n";
	for (size_t i = 0; i < serv._channels.size(); i++)
	{
		std::cout << serv._channels[i].getName() << std::endl;
	}
}

void Command::join(Server &serv, std::string &nick, int it)
{
	if (checkNumberParam(serv, nick, it) == false)
		return ;
	checkEntryChannel(serv, nick, it);
	serv._channels[0].printMap();
}
