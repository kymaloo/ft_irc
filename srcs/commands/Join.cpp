#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

bool Command::isNameChannelValid(Server &serv, std::string &channel, int itClient)
{
	// std::string message;
	// pollfd *pfds = serv.getPfds();
    if (channel.empty() || channel[0] != '#')
	{
		Reply::sendError(serv, 403, itClient, serv.getClientNick(itClient), channel);
		// message = Reply::ERR_NOSUCHCHANNEL(serv.getServName(), nick, channel);
		// std::cerr << message << std::endl;
		// send(pfds[it].fd, message.c_str(), message.size(), 0);
		return (false);
	}
	return (true);
}

bool Command::isMdpValid(Server &serv, std::string &channel, int itClient)
{
	// std::string message;
	// pollfd *pfds = serv.getPfds();
    if (channel.empty() || channel[0] != '#')
	{
		Reply::sendError(serv, 475, itClient, serv.getServName(), channel);
		// message = Reply::ERR_BADCHANNELKEY(serv.getServName(), channel);
		// std::cerr << message << std::endl;
		// send(pfds[it].fd, message.c_str(), message.size(), 0);
		return (false);
	}
	return (true);
}

bool Command::checkNumberParam(Server &serv, int itClient)
{
	// std::string message;
	// pollfd *pfds = serv.getPfds();
	if (_params[0].empty())
	{
		Reply::sendError(serv, 461, itClient, _commandName, "NULL");
		// message = Reply::ERR_NEEDMOREPARAMS(serv.getServName(), serv.getClientNick(itClient), _commandName);
		// std::cerr << message << std::endl;
		// send(pfds[itClient].fd, message.c_str(), message.size(), 0);
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

void Command::checkEntryChannel(Server &serv, int itClient)
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
		if (isNameChannelValid(serv, vecChannel[i], itClient) == false)
		{
			if (j != vecMdp.size())
				j++;
			continue;
		}
		if (_params.size() > 1 && !_params[1].empty())
		{
			if (isChannelIntoList(serv, vecChannel[i]) == true && serv.getIsPasswordChannel(getIteratorChannel(serv, vecChannel[i])) == true)
			{
				if (serv.getPasswordChannel(getIteratorChannel(serv, vecChannel[i])) == vecMdp[j])
					serv.setNewUser(getIteratorChannel(serv, vecChannel[i]), serv.getClientfd(itClient));
			}
			else
			{ 
				std::cout << "La je comprend pas je creer le channel ?\n";
				isMdpValid(serv, vecChannel[i], itClient);
			}
			if (j != vecMdp.size())
				j++;
		}
		else
		{
			if (isChannelIntoList(serv, vecChannel[i]) == true)
			{
				serv.setNewUser(getIteratorChannel(serv, vecChannel[i]), serv.getClientfd(itClient));
			}
			if (isChannelIntoList(serv, vecChannel[i]) == false)
			{
				serv.setNewChannel(vecChannel[i], serv.getClientfd(itClient), true);
			}
		}
	}
	// std::cout << "Channel list :\n";
	// for (size_t i = 0; i < serv.getChannelSize(); i++)
	// {
	// 	if (serv.getChannelName(i).empty() == false)
	// 		std::cout << serv.getChannelName(i) << std::endl;
	// }
}



void Command::join(Server &serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (checkNumberParam(serv, itClient) == false)
		return ;
	checkEntryChannel(serv, itClient);

	for (size_t i = 0; i < serv.getChannelSize(); i++)
		serv.printMapChannel(i);
}
