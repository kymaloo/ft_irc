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
	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.getChannelName(i) == vecChannel)
			return (true);
	}
	return (false);
}

size_t Command::getIteratorChannel(Server &serv, std::string &vecChannel)
{
	for (size_t i = 0; i != serv.getChannelSize(); i++)
	{
		if (serv.getChannelName(i) == vecChannel)
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
			if (isChannelIntoList(serv, vecChannel[i]) == true && serv.getIsPasswordChannel(getIteratorChannel(serv, vecChannel[i])) == true)
			{
				// if (serv.getChannelName(i)[getIteratorChannel(serv, vecChannel[i])].getPassWorld() == vecMdp[j])
				// 	serv.getChannelName(i)[getIteratorChannel(serv, vecChannel[i])].addClient(user);
				if (serv.getPasswordChannel(getIteratorChannel(serv, vecChannel[i])) == vecMdp[j])
					serv.setNewUser(getIteratorChannel(serv, vecChannel[i]), user);
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
			if (isChannelIntoList(serv, vecChannel[i]) == true)
			{
				serv.setNewUser(getIteratorChannel(serv, vecChannel[i]), user);
			}
			if (isChannelIntoList(serv, vecChannel[i]) == false)
			{
				serv.setNewChannel(vecChannel[i], user, true);
				//serv.printMapChannel(serv.getClientfd(i));
			}
		}
	}
	// std::cout << "C'est moi qui print grosse merde\n";
	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// {
	// 	std::cout << serv.getChannelName(i) << std::endl;
	// }
}

void Command::join(Server &serv, std::string &nick, int it)
{
	if (checkNumberParam(serv, nick, it) == false)
		return ;
	checkEntryChannel(serv, nick, it);
	std::cout << "size: " << serv.getChannelSize() << std::endl;
	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// 	serv.printMapChannel(i);
}
