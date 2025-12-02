#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

bool Command::isNameChannelValid(Server &serv, std::string &channel, int itClient)
{
    if (channel.empty() || channel[0] != '#')
	{
		Reply::sendError(serv, 403, itClient, serv.getClientNick(itClient), channel);
		return (false);
	}
	return (true);
}

bool Command::isMdpValid(Server &serv, std::string &channel, int itClient)
{
    if (channel.empty() || channel[0] != '#')
	{
		Reply::sendError(serv, 475, itClient, serv.getServName(), channel);
		return (false);
	}
	return (true);
}

bool Command::checkNumberParam(Server &serv, int itClient)
{
	if (_params[0].empty())
	{
		Reply::sendError(serv, 461, itClient, _commandName, "NULL");
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
	
	std::vector<std::string> vecNameChannel;
	std::vector<std::string> vecPassword;
	size_t j = 0;

	if (!_params[0].empty())
		vecNameChannel = split(_params[0]);
	if (_params.size() > 1 && !_params[1].empty())
		vecPassword = split(_params[1]);

	for (size_t itChannel = 0; itChannel != vecNameChannel.size(); itChannel++)
	{
		if (isNameChannelValid(serv, vecNameChannel[itChannel], itClient) == false)
		{
			if (j != vecPassword.size())
				j++;
			continue;
		}
		if (isChannelIntoList(serv, vecNameChannel[itChannel]) == false)
			serv.setNewChannel(vecNameChannel[itChannel], serv.getClientfd(itClient), true);

		else if (_params.size() > 1 && !_params[1].empty() && serv.getChannelMode('k', itChannel) == true)
		{
			if (serv.getPasswordChannel(getIteratorChannel(serv,vecNameChannel[itChannel])) == vecPassword[j])
				serv.setNewUser(getIteratorChannel(serv,vecNameChannel[itChannel]), serv.getClientfd(itClient));

		}
		// else if (serv.getChannelMode('i', itChannel) == true && //fonction: )
		// {

		// }
		else if (isChannelIntoList(serv,vecNameChannel[itChannel]) == true && serv.getChannelMode('k', itChannel) == false)
			serv.setNewUser(getIteratorChannel(serv,vecNameChannel[itChannel]), serv.getClientfd(itClient));
		if (j != vecPassword.size())
			j++;
	}
}

void Command::join(Server &serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (checkNumberParam(serv, itClient) == false)
		return ;
	checkEntryChannel(serv, itClient);
}

void Command::printChannel(Server &serv)
{
	for (size_t i = 0; i < serv.getChannelSize(); i++)
		serv.printMapChannel(i);
}
