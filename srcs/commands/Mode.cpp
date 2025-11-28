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
	else if (params[0][0] == '#')
	{
		if (serv.doesChannelExist(params[0]) == false)
		{
			Reply::sendError(serv, 403, itClient, params[0], "NULL");
			return false;
		}
		if (serv.isClientOnChannel(serv.getChannelIterator(params[0]), serv.getClientfd(itClient)) == false)
		{
			Reply::sendError(serv, 442, itClient, params[0], "NULL");
			return false;
		}
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
	while (i < params.size())
		modeParams.push_back(params[i]);
	return modeParams;
}

void handleChannelModes(Server& serv, std::vector<std::string> modes, std::vector<std::string> params, int itChannel, int itClient)
{
	bool modeState = false;
	size_t itParams = 0;

	if (serv.isOpInChannel(itChannel, serv.getClientfd(itClient)) == false)
	{
		Reply::sendError(serv, 482, itClient, serv.getChannelName(itChannel), "NULL");
		return;
	}
	for (size_t itMVec = 0; itMVec < modes.size(); itMVec++)
	{
		modeState = (modes[itMVec][0] == '+');
		for (size_t itModes = 1; itModes < modes[itMVec].size(); itModes++)
		{
			itParams = 1;
			char mode = modes[itMVec][itModes];
			if (mode == 'i' || mode == 't')
				serv.setChannelMode(mode, modeState, itChannel, "");
			else if (mode == 'k' || mode == 'l')
			{
				if (modeState == true && (params.empty() == true || params[itMVec].empty() == true || itParams > params[itMVec].size()))
				{
					Reply::sendError(serv, 461, itClient, "NULL", "NULL");
					continue;				
				}
				if (mode == 'k' && modeState == serv.getChannelMode(mode, itChannel) && modeState == true)
					Reply::sendError(serv, 467, itClient, "NULL", "NULL");
				serv.setChannelMode(mode, modeState, itChannel, params[itParams]);
				if (itParams <= params[itMVec].size())
					itParams++;
			}
			else if (mode == 'o')
				serv.setChannelOperators(modeState, itChannel, ToolBox::split(params[itParams], ','));
			else
				Reply::sendError(serv, 472, itClient, std::string(&mode), "NULL");
		}
		
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


//! # ------ ERR/RPL list ------ #
			//? 461 - ERR_NEEDMOREPARAMS              (234) - RPL_CHANNELMODEIS
			//? 482 - ERR_CHANOPRIVSNEEDED            401 - ERR_NOSUCHNICK
			//? 442 - ERR_NOTONCHANNEL                467 - ERR_KEYSET
			// RPL_BANLIST                     RPL_ENDOFBANLIST
			//? 472 - ERR_UNKNOWNMODE                 403 - ERR_NOSUCHCHANNEL

			//? 502 - ERR_USERSDONTMATCH              (221) - RPL_UMODEIS
			//? 501 - ERR_UMODEUNKNOWNFLAG

/*
Channels

	TODO i - drapeau de canal accessible uniquement sur invitation
	TODO t - drapeau de sujet de canal modifiable uniquement par les opérateurs
	TODO k - définit la clé du canal (mot de passe)
	TODO o - donne/retire les privilèges d'opérateur de canal - (max 3 args)
	TODO l - définit le nombre maximal de personnes dans un canal
	403 - ERR_NOSUCHCHANNEL		-
	482 - ERR_CHANOPRIVSNEEDED	-
	442 - ERR_NOTONCHANNEL		-
	467 - ERR_KEYSET			-

	(234) - RPL_CHANNELMODEIS
Clients
	401 - ERR_NOSUCHNICK		-
	502 - ERR_USERSDONTMATCH

	(221) - RPL_UMODEIS
Both
	461 - ERR_NEEDMOREPARAMS	-
	472 - ERR_UNKNOWNMODE		-
	501 - ERR_UMODEUNKNOWNFLAG	
*/







// 4.2.3 Message MODE

// Commande: MODE

// La commande MODE est une commande à utilisation duale sur IRC. \
// Elle permet aussi bien de changer les modes des utilisateurs que ceux des canaux.\
// La raison à ce choix est qu'un jour les pseudonymes deviendront obsolètes \
// et la propriété équivalente sera le canal.

// Lors du traitement des messages MODE, il est recommandé de commencer par décomposer le message en entier, puis de réaliser les modifications résultantes.

//! # ------ 4.2.3.1 Les modes des canaux ------ #

// Paramètres: <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]

// La commande MODE permet aux opérateurs de canal de changer les caractéristiques de 'leur' canal. Les serveurs doivent aussi pouvoir changer les modes du canal, de façon à pouvoir créer des opérateurs.

// Les modes disponibles pour les canaux sont les suivants :
//TODO i - drapeau de canal accessible uniquement sur invitation
//TODO t - drapeau de sujet de canal modifiable uniquement par les opérateurs
//TODO k - définit la clé du canal (mot de passe)
//TODO o - donne/retire les privilèges d'opérateur de canal
//TODO l - définit le nombre maximal de personnes dans un canal
// // p - drapeau de canal privé
// // s - drapeau de canal secret
// // n - pas de messages dans un canal provenant de clients à l'extérieur du canal
// // m - canal modéré
// // b - définit un masque de bannissement pour interdire l'accès à des utilisateurs
// // v - donne/retire la possibilité de parler dans un canal modéré

//? Lors de l'utilisation des options 'o' et 'b',
//? le nombre de paramètres est restreint à trois par commande,
//? et ce pour n'importe quelle combinaison de 'o' et de 'b'. 

//! # ------ 4.2.3.2 Modes des utilisateurs ------ #

// Paramètres: <pseudonyme> {[+|-]|i|w|s|o}

// Les modes utilisateurs sont typiquement des modifications qui affectent la façon dont le client est vu par les autres, ou quels types de messages sont reçus. Une commande MODE n'est acceptée que si l'expéditeur du message et le pseudonyme donné en paramètre sont les mêmes.

// Les modes disponibles sont :
//TODO i - marque un utilisateur comme invisible ;
//TODO o - drapeau d'opérateur.
// // s - marque un utilisateur comme recevant les notifications du serveur ;
// // w - l'utilisateur reçoit les WALLOPs ;

//// D'autres modes seront disponibles plus tard.

// Si un utilisateur tente de devenir opérateur en utilisant le drapeau "+o", la tentative doit être ignorée. Par contre, il n'y a pas de restriction à ce que quelqu'un se 'deoppe' (en utilisant "-o"). 

//! # ------ ERR/RPL list ------ #
			//? 461 - ERR_NEEDMOREPARAMS              (234) - RPL_CHANNELMODEIS
			//? 482 - ERR_CHANOPRIVSNEEDED            401 - ERR_NOSUCHNICK
			//? 442 - ERR_NOTONCHANNEL                (467) - ERR_KEYSET
			// RPL_BANLIST                     RPL_ENDOFBANLIST
			//? (472) - ERR_UNKNOWNMODE                 403 - ERR_NOSUCHCHANNEL

			//? (502) - ERR_USERSDONTMATCH              (221) - RPL_UMODEIS
			//? (501) - ERR_UMODEUNKNOWNFLAG

/*
TODO : implement following replyes (maybe)

?(324) RPL_CHANNELMODEIS
    "<canal> <mode> <paramètres de mode>"

?(221) RPL_UMODEIS
    "<chaîne de mode utilisateur>"

*/

/*
The subject demands :
	· i: Set/remove Invite-only channel
	· t: Set/remove the restrictions of the TOPIC command to channel operators
	· k: Set/remove the channel key (password)
	· o: Give/take channel operator privilege
	· l: Set/remove the user limit to channel
*/
