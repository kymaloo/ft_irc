#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"

static bool isParamsGood(std::vector<std::string> vec)
{
	std::vector<std::string> vecChannelName;
	std::vector<std::string> vecClientName;

	if (vec.size() != 2)
		return false;
	if (!vec[0].empty())
		vecChannelName = split(vec[0]);
	if (vecChannelName.size() != 1)
		return false;
	if (!vec[1].empty())
		vecClientName = split(vec[1]);
	if (vecClientName.size() != 1)
		return false;
	return true;
}

// void Command::invite(Server &serv, int fdClient)
// {
// 	int			itClient;
// 	std::string	channelName;
// 	std::string	clientName;
	
// 	if (isParamsGood(_params) == false)
// 		return;
// 	clientName = _params[0];
// 	channelName = _params[1];
// 	// if (serv.isClientNicknameExiste(clientName) == false)
// 	// 	return ;
// 	itClient = serv.getClientIt(serv.getClientfd(clientName));
// 	if (checkNumberParam(serv, itClient) == false)
// 		return ;
// 	if (serv.getClientNick(serv.getClientIt(fdClient)) == "")
// 	{
// 		Reply:: ERR_NOSUCHNICK(serv.getServName(), clientName, channelName);
// 		return ;
// 	}
// 	if (isNameChannelValid(serv, channelName, serv.getClientIt(fdClient)) == false)
// 		return ;




	// if (serv.isOpInChannel(serv.getChannelIterator(channelName), fdClient) == false)
	// {
	// 	Reply::sendError(serv, 482, serv.getClientIt(fdClient), channelName, "NULL");
	// 	return ;
	// }
// 	if (isChannelIntoList(serv, channelName) == false)
// 		serv.addChannelInvitedClient(channelName, CHANNEL_NOT_EXIST, serv.getClientfd(clientName));
// 	else
// 		serv.addChannelInvitedClient(channelName, serv.getChannelSize() - 1, serv.getClientfd(clientName));




	// if (serv.isClientOnChannel(serv.getChannelIterator(channelName), fdClient) == false)
	// {
	// 	std::cout << "isClientOnChannel == false\n";
	// 	Reply::ERR_NOTONCHANNEL(serv.getServName(), serv.getClientNick(serv.getClientIt(fdClient)), channelName);
	// 	return ;
	// }
	// if (serv.isClientOnChannel(serv.getChannelIterator(channelName), serv.getClientfd(clientName)) == true)
	// {
	// 	std::cout << "isClientOnChannel == true l'inviter\n";
	// 	Reply::ERR_USERONCHANNEL(serv.getServName(), clientName, channelName);
	// 	return ;
	// }

	
	
// 	serv.setChannelMode('i', true, serv.getChannelSize() - 1, "NULL");
// 	std::string msg = ":" + serv.getClientNick(serv.getClientIt(fdClient)) + "!" + serv.getClientUser(serv.getClientIt(fdClient)) + "@localhost INVITE " + clientName + " " + channelName + "\r\n";
// 	send(serv.getClientfd(itClient), msg.c_str(), msg.size(), 0);
// 	Reply::RPL_INVITING(serv.getServName(), channelName, clientName);
// }


// ! Step 1 : le channel doit exister
// ! Step 2 : seule les membre du channel peuvent inviter 
// ! Step 3 : si mode +i que op pour inviter 
// ! Step 4 : si user deja sur canal

void Command::invite(Server &serv, int fdClient)
{
	int			itClient;
	std::string	channelName;
	std::string	clientName;
	
	if (isParamsGood(_params) == false)
		return;
	clientName = _params[0];
	channelName = _params[1];

	itClient = serv.getClientIt(serv.getClientfd(clientName));
	if (checkNumberParam(serv, itClient) == false)
		return ;
	if (isChannelIntoList(serv, channelName) == false)
	{
		Reply::sendError(serv, 403, serv.getClientIt(fdClient), channelName, "NULL");
		return ;
	}
	if (serv.isClientOnChannel(serv.getChannelIterator(channelName), fdClient) == false)
	{
		Reply::ERR_NOTONCHANNEL(serv.getServName(), serv.getClientNick(serv.getClientIt(fdClient)), channelName);
		return ;
	}
	if (serv.getChannelMode('i', serv.getChannelIterator(channelName) == true) && serv.isOpInChannel(serv.getChannelIterator(channelName), fdClient) == false)
	{
		Reply::sendError(serv, 482, serv.getClientIt(fdClient), channelName, "NULL");
		return ;
	}
	if (serv.isClientOnChannel(serv.getChannelIterator(channelName), serv.getClientfd(clientName)) == true)
	{
		Reply::ERR_USERONCHANNEL(serv.getServName(), clientName, channelName);
		return ;
	}
	if (serv.isClientInvitedInChannel(serv.getChannelIterator(channelName), itClient) == true)
		return ;
	serv.addChannelInvitedClient(channelName, serv.getChannelSize() - 1, serv.getClientfd(clientName));
	std::string msg = ":" + serv.getClientNick(serv.getClientIt(fdClient)) + "!" + serv.getClientUser(serv.getClientIt(fdClient)) + "@localhost INVITE " + clientName + " " + channelName + "\r\n";
	send(serv.getClientfd(itClient), msg.c_str(), msg.size(), 0);
	Reply::RPL_INVITING(serv.getServName(), channelName, clientName);
}
