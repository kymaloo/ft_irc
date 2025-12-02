#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

//Returns 1 if the target is a client, 2 if the target is a channel and 0 if error
bool checkParams(Server& serv, std::string command, std::vector<std::string> params, int itClient)
{
	if (params.empty() == true || params.size() < 2)
	{
		Reply::sendError(serv, 461, itClient, command, "NULL");
		return false;
	}
	if (params[0][0] != '#' || serv.doesChannelExist(params[0]) == false)
	{
		Reply::sendError(serv, 403, itClient, params[0], "NULL");
		return false;
	}
	if (serv.isClientOnChannel(serv.getChannelIterator(params[0]), serv.getClientfd(itClient)) == false)
	{
		Reply::sendError(serv, 442, itClient, params[0], "NULL");
		return false;
	}

	return true;
}

std::vector<std::string> getModes(std::vector<std::string> params)
{
	std::vector<std::string> modes;
	for (size_t i = 1; i < params.size(); i++)
	{
		if (params[i][0] == '+' || params[i][0] == '-')
			modes.push_back(params[i]);
		else
			break;
	}
	return modes;
}

std::vector<std::string> getModeParams(std::vector<std::string> params)
{
	std::vector<std::string> modeParams;
	size_t i = 1;
	while (i < params.size() && (params[i][0] == '+' || params[i][0] == '-'))
		i++;
	if (params.size() > 0)
	{
		while (i < params.size())
		{
			modeParams.push_back(params[i]);
			i++;
		}
	}
	return modeParams;
}

std::string joinModes(std::vector<std::string> modes)
{
	std::string PositiveModes;
	std::string negativeModes;
	for (size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i][0] == '+')
		{
			for (size_t j = 1; j < modes[i].size(); j++)
			{
				if (negativeModes.find(modes[i][j]) != std::string::npos)
					negativeModes.erase(negativeModes.find(modes[i][j]), 1);
				PositiveModes += modes[i].substr(1);
			}
		}
		else if (modes[i][0] == '-')
		{
			for (size_t j = 1; j < modes[i].size(); j++)
			{
				if (PositiveModes.find(modes[i][j]) != std::string::npos)
					PositiveModes.erase(PositiveModes.find(modes[i][j]), 1);
				negativeModes += modes[i].substr(1);
			}
		}
	}
	for (size_t i = 0; i != std::string::npos; i = negativeModes.find_first_not_of("itklo", i))
		if (i != std::string::npos)
			negativeModes.erase(i, 1);
	for (size_t i = 0; i != std::string::npos; i = PositiveModes.find_first_not_of("itklo", i))
		if (i != std::string::npos)
			PositiveModes.erase(i, 1);
	return "+" + PositiveModes + " -" + negativeModes;
}

void handleChannelModes(Server& serv, std::vector<std::string> modes, std::vector<std::string> params, int itChannel, int itClient)
{
	bool modeState = false;
	size_t itParams = 0;
	std::string joinedModes = joinModes(modes);
	std::string operators;

	if (serv.doesChannelExist(itChannel) == true && serv.isOpInChannel(itChannel, serv.getClientfd(itClient)) == false)
	{
		Reply::sendError(serv, 482, itClient, serv.getChannelName(itChannel), "NULL");
		return;
	}
	for (size_t itMVec = 0; itMVec < modes.size(); itMVec++)
	{
		modeState = (modes[itMVec][0] == '+');
		for (size_t itModes = 1; itModes < modes[itMVec].size(); itModes++)
		{
			itParams = 0;
			char mode = modes[itMVec][itModes];
			if (mode == 'i' || mode == 't')
				serv.setChannelMode(mode, modeState, itChannel, "");
			else if (mode == 'k' || mode == 'l' || mode == 'o')
			{
				if (modeState == true && (params.empty() == true || params[itMVec].empty() == true || itParams > params[itMVec].size()))
				{
					Reply::sendError(serv, 461, itClient, "NULL", "NULL");
					continue;				
				}
				if (mode == 'o')
					operators = serv.setChannelOperators(modeState, itChannel, ToolBox::split(params[itParams], ','));
				else if (mode == 'k' && modeState == serv.getChannelMode(mode, itChannel) && modeState == true)
					Reply::sendError(serv, 467, itClient, "NULL", "NULL");
				serv.setChannelMode(mode, modeState, itChannel, params[itParams]);
				if (itParams <= params[itMVec].size())
					itParams++;
			}
			else
				Reply::sendError(serv, 472, itClient, std::string(&mode), "NULL");
		}
		Reply::sendModes(serv, itChannel, joinedModes, operators);
	}
}

void Command::mode(Server& serv, int fdClient)
{
	int itClient = serv.getClientIt(fdClient);
	if (checkParams(serv, _commandName, _params, itClient) == false)
		return ;
	handleChannelModes(serv, getModes(_params), getModeParams(_params), serv.getChannelIterator(_params[0]), itClient);
	return ;
}

/*


TODO mode +i : degager les gens en trop


	(234) - RPL_CHANNELMODEIS

	403 - ERR_NOSUCHCHANNEL		-
	482 - ERR_CHANOPRIVSNEEDED	-
	442 - ERR_NOTONCHANNEL		-
	467 - ERR_KEYSET			-
	401 - ERR_NOSUCHNICK		-
	461 - ERR_NEEDMOREPARAMS	-
	472 - ERR_UNKNOWNMODE		-

TODO : implement following replyes

?(324) RPL_CHANNELMODEIS
    "<canal> <mode> <paramètres de mode>"
*/
