#include "../../includes/server.hpp" 
#include "../../includes/Command.hpp"
#include "../../includes/reply.hpp"

std::string	Command::user(Server &serv, int it)
{
	//TODO corriger cette partie

	for (size_t i = 0; i < _params.size(); i++)
	{
		std::cout << "Param " << i << " : " << _params[i] << std::endl;
	}
	if (_params.size() < 3)
	{
		Reply::sendError(serv, 461, it);
		return "ERROR";
	}

	// send welcome only if both nick and user are set for the first time
	if (serv.getClientUser(it).empty())
	{
		serv.setClientUser(_params[0], it);
		serv.setClientReal(_params[_params.size() - 1], it);
		std::cout << "User set to : " << serv.getClientUser(it) << std::endl;
		std::cout << "Real set to : " << serv.getClientReal(it) << std::endl;
		if (!serv.getClientNick(it).empty())
		{
			std::cout << GREEN << "Client logged.\n" << WHITE;
			Reply::welcomeClient(serv, it);
		}
	}
	return serv.getClientUser(it);
}


//	username = _params[0]
//	realname = _params[last]
//	Commande: USER
//	Paramètres: <nom d'utilisateur> <hôte> <nom de serveur> :<nom réel>