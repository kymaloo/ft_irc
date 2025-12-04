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

bool Command::isClientInvited(Server &serv, int itChannel, int itClient)
{
	if (serv.isClientInvitedInChannel(itChannel, itClient) == true)
		return true;
	// Reply
	return false;
}

void parseParamsJoin(std::vector<std::string> &_params, std::vector<std::string> &name, std::vector<std::string> &password)
{
	if (!_params[0].empty())
		name = split(_params[0]);
	if (_params.size() > 1 && !_params[1].empty())
		password = split(_params[1]);
}

bool Command::joinInvite(Server &serv, int itChannel, int itClient)
{
	if (isClientInvited(serv, itChannel, itClient) == false)
	{
		Reply::sendError(serv, 473, itClient, serv.getChannelName(itChannel), "NULL");
		return false;
	}
	return true;
}

bool Command::joinLimite(Server &serv, int itChannel, int itClient)
{
	if (isClientInvited(serv, itChannel, itClient) == false)
	{
		if (static_cast<int>(serv.getChannelSize()) >= serv.getChannelLimit(itChannel))
		{
			Reply::sendError(serv, 472, itClient, serv.getChannelName(itChannel), "NULL");
			return false;
		}
	}
	return true;
}

bool Command::joinPassword(Server &serv, std::vector<std::string> &vecPassword, std::vector<std::string> &vecNameChannel, int itChannel, int itClient, size_t iteratorVecPassword)
{
	if (isClientInvited(serv, itChannel, itClient) == false )
	{
		if (vecPassword.empty() || iteratorVecPassword > vecPassword.size() || serv.getPasswordChannel(getIteratorChannel(serv,vecNameChannel[itChannel])) != vecPassword[iteratorVecPassword])
		{
			Reply::sendError(serv, 475, itClient, serv.getChannelName(itChannel), "NULL");
			return false;
		}
	}
	return true;
}


void Command::checkEntryChannel(Server &serv, int itClient)
{
	
	std::vector<std::string>	vecNameChannel;
	std::vector<std::string>	vecPassword;
	bool						isOk = true;
	size_t j = 0;

	parseParamsJoin(_params, vecNameChannel, vecPassword);

	for (size_t itChannel = 0; itChannel != vecNameChannel.size(); itChannel++)
	{
		isOk = true;
		if (isNameChannelValid(serv, vecNameChannel[itChannel], itClient) == false)
			isOk = false;
		if (isChannelIntoList(serv, vecNameChannel[itChannel]) == false && isOk == true)
		{
			serv.setNewChannel(vecNameChannel[itChannel], serv.getClientfd(itClient), true);
			std::string msg;
			msg += ":" + serv.getClientNick(itClient) + "!" + serv.getClientUser(itClient) + " JOIN " + vecNameChannel[itChannel] + "\r\n";
			serv.sendToChannelWithoutPrivateMsg(itChannel, msg);
			Reply::sendRplNamereply(serv, itClient, vecNameChannel[itChannel]);
			Reply::sendRplEndOfName(serv, itClient, vecNameChannel[itChannel]);
			isOk = false;
		}
		else
		{
			if (serv.getChannelMode('l', itChannel) == true && isOk == true)
				isOk = joinLimite(serv, itChannel, itClient);
			if (serv.getChannelMode('i', itChannel) == true && isOk == true)
			{
				isOk = joinInvite(serv, itChannel, itClient);
				if (serv.getChannelMode('k', itChannel) == true && isOk == true)
					isOk = joinPassword(serv, vecPassword, vecNameChannel, itChannel, itClient, j);
				std::cout << "test\n";
			}
			else if (serv.getChannelMode('k', itChannel) == true && isOk == true && serv.getChannelMode('l', itChannel) == false)
				isOk = joinPassword(serv, vecPassword, vecNameChannel, itChannel, itClient, j);
		}
		if (isOk == true)
		{
			serv.setNewUser(getIteratorChannel(serv, vecNameChannel[itChannel]), serv.getClientfd(itClient));
			std::string msg;
			msg += ":" + serv.getClientNick(itClient) + "!" + serv.getClientUser(itClient) + " JOIN " + vecNameChannel[itChannel] + "\r\n";
			serv.sendToChannelWithoutPrivateMsg(itChannel, msg);
			Reply::sendRplNamereply(serv, itClient, vecNameChannel[itChannel]);
			Reply::sendRplEndOfName(serv, itClient, vecNameChannel[itChannel]);
		}
		j++;
	}
}

// void Command::checkEntryChannel(Server &serv, int itClient)
// {
	
// 	std::vector<std::string> vecNameChannel;
// 	std::vector<std::string> vecPassword;
// 	size_t j = 0;

// 	parseParamsJoin(_params, vecNameChannel, vecPassword);

// 	for (size_t itChannel = 0; itChannel != vecNameChannel.size(); itChannel++)
// 	{
// 		if (isNameChannelValid(serv, vecNameChannel[itChannel], itClient) == false)
// 		{
// 			j++;
// 			continue;
// 		}
// 		if (isChannelIntoList(serv, vecNameChannel[itChannel]) == false)
// 			serv.setNewChannel(vecNameChannel[itChannel], serv.getClientfd(itClient), true);
// 		else
// 		{
// 			if (serv.getChannelMode('l', itChannel) == true)
// 			{
// 				//reply
// 				std::cout << "O veux pas de toi\n";
// 				j++;
// 				continue ;
// 			}
// 			if (serv.getChannelMode('i', itChannel) == true)
// 			{
// 				if (isClientInvited(serv, itChannel, itClient) == false)
// 				{
// 					std::cout << "Mouhaha tu n'es pas inviter povre mer2\n";
// 					j++;
// 					continue ; 
// 				}
// 				else
// 					serv.setNewUser(getIteratorChannel(serv,vecNameChannel[itChannel]), serv.getClientfd(itClient));
// 			}
// 			if (_params.size() > 1 && !_params[1].empty() && serv.getChannelMode('k', itChannel) == true)
// 			{
// 				if (j < vecPassword.size() && serv.getPasswordChannel(getIteratorChannel(serv,vecNameChannel[itChannel])) == vecPassword[j])
// 					serv.setNewUser(getIteratorChannel(serv,vecNameChannel[itChannel]), serv.getClientfd(itClient));
// 			}
// 			else
// 				serv.setNewUser(getIteratorChannel(serv,vecNameChannel[itChannel]), serv.getClientfd(itClient));
// 		}
// 		j++;
// 	}
// }

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
