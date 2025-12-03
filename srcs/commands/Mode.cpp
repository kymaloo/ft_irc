#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

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
	int itChannel = serv.getChannelIterator(params[0]);
	if (serv.isClientOnChannel(itChannel, serv.getClientfd(itClient)) == false)
	{
		Reply::sendError(serv, 442, itClient, params[0], "NULL");
		return false;
	}
	if (serv.doesChannelExist(itChannel) == true && serv.isOpInChannel(itChannel, serv.getClientfd(itClient)) == false)
	{
		Reply::sendError(serv, 482, itClient, serv.getChannelName(itChannel), "NULL");
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

void addModes(bool* modeState, std::string* a, std::string* b, std::string modes)
{
	for (size_t j = 0; j < modes.size(); j++)
	{
		if (modes[j] == '+' || modes[j] == '-')
		{
			*modeState = (modes[j] == '+');
			continue;
		}
		if (b->find(modes[j]) != std::string::npos)
			b->erase(b->find(modes[j]), 1);
		*a += modes[j];
	}
}

void cleanModesList(std::string* modes)
{
	for (size_t i = modes->find_first_not_of("itklo"); i != std::string::npos; i = modes->find_first_not_of("itklo"))
		if (i != std::string::npos)
			modes->erase(i, 1);
}

std::string joinModes(std::vector<std::string> modes)
{
	std::string positiveModes;
	std::string negativeModes;
	std::string joinedModes;
	bool modeState;

	for (size_t i = 0; i < modes.size(); i++)
	{
		modeState = (modes[i][0] == '+');
		if (modeState == true)
			addModes(&modeState, &positiveModes, &negativeModes, modes[i]);
		else if (modeState == false)
			addModes(&modeState, &negativeModes, &positiveModes, modes[i]);
	}
	cleanModesList(&positiveModes);
	cleanModesList(&negativeModes);
	if (positiveModes.empty() == false)
		joinedModes += "+" + positiveModes;
	if (negativeModes.empty() == false)
		joinedModes += "-" + negativeModes;
	return joinedModes;
}

// void setChannelModes(Server& serv, std::string modes, std::vector<std::string> params, int itChannel, int itClient)
// {

// }

bool isModeKnown(char mode)
{
	if (mode == 'i' || mode == 't' || mode == 'k' || mode == 'l' || mode == 'o' || mode == '+' || mode == '-')
		return true;
	return false;
}

bool doesNeedParam(bool modeState, char mode)
{
	if (modeState == false)
	{
		if (mode == 'o')
			return true;
		return false;
	}
	if (mode == 'i' || mode == 't')
		return false;
	return true;
}

void handleChannelModes(Server& serv, std::vector<std::string> modes, std::vector<std::string> params, int itChannel, int itClient)
{
	bool modeState = false;
	size_t itParams = 0;
	std::string joinedModes = joinModes(modes);
	std::string operators;

	for (size_t itMVec = 0; itMVec < modes.size(); itMVec++)
	{
		modeState = (modes[itMVec][0] == '+');
		for (size_t itModes = 1; itModes < modes[itMVec].size(); itModes++)
		{

			char mode = modes[itMVec][itModes];
			if (isModeKnown(mode) == false)
			{
				if (mode == '+' || mode == '-')
					modeState = (mode == '+');
				else
					Reply::sendError(serv, 472, itClient, std::string(&mode), "NULL");
				continue;
			}
			if (doesNeedParam(modeState, mode) == true)
			{
				if (params.empty() || itParams > params.size())
				{
					Reply::sendError(serv, 461, itClient, "NULL", "NULL");
					continue;				
				}
				if (mode == 'k' && modeState & serv.getChannelMode(mode, itChannel) & true)
				{
					Reply::sendError(serv, 467, itClient, "NULL", "NULL");
					continue;
				}
				if (mode == 'o')
					operators = serv.setChannelOperators(modeState, itClient, itChannel, ToolBox::split(params[itParams], ','));
				else
					serv.setChannelMode(mode, modeState, itChannel, params[itParams]);
				itParams++;
			}
			else
				serv.setChannelMode(mode, modeState, itChannel, "");
		}
	}
	Reply::sendModes(serv, itChannel, joinedModes, operators);
}

void Command::mode(Server& serv, int fdClient)
{
	int itChannel = serv.getChannelIterator(_params[0]);
	int itClient = serv.getClientIt(fdClient);
	if (checkParams(serv, _commandName, _params, itClient) == false)
		return ;
	handleChannelModes(serv, getModes(_params), getModeParams(_params), itChannel, itClient);
	std::cout << "channel mode l = " << serv.getChannelMode('l', itChannel) << "\nand limit is " << serv.getChannelLimit(itChannel) << std::endl;
	_params[0] = serv.getChannelName(itChannel);
	if (serv.getChannelMode('l', itChannel) == true)
		while (serv.getChannelLimit(itChannel) < serv.getChannelSize(itChannel))
			part(serv, serv.getChannelLastclientFd(itChannel));
	return ;
}
