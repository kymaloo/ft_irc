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
			if (mode == '+' || mode == '-')
				modeState = (modes[itMVec][itModes] == '+');
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
					operators = serv.setChannelOperators(modeState, itClient, itChannel, ToolBox::split(params[itParams], ','));
				else if (mode == 'k' && modeState == serv.getChannelMode(mode, itChannel) && modeState == true)
					Reply::sendError(serv, 467, itClient, "NULL", "NULL");
				serv.setChannelMode(mode, modeState, itChannel, params[itParams]);
				if (itParams <= params[itMVec].size())
					itParams++;
			}
			else
				Reply::sendError(serv, 472, itClient, std::string(&mode), "NULL");
		}
	}
	Reply::sendModes(serv, itChannel, joinedModes, operators);
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


*/
