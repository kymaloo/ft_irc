#ifndef REPLY_HPP
# define REPLY_HPP

# include "forward.hpp"
# include <string>

class Reply
{
	public:
		// === Réponses de connexion ===
		static std::string RPL_WELCOME(const std::string& server, const std::string& user, const std::string& nick, const std::string& host);
		static std::string RPL_YOURHOST(const std::string& server, const std::string& version);
		static std::string RPL_CREATED(const std::string& server, const std::string& date);
		static std::string RPL_MYINFO(const std::string& server, const std::string& version, const std::string& userModes, const std::string& channelModes);

		// === Commandes de canaux ===

		// static std::string RPL_UMODEIS(const std::string& server, const std::string& channel);
		static std::string RPL_CHANNELMODEIS(const std::string& server, const std::string& channel);

		static std::string RPL_NOTOPIC(const std::string& server, const std::string& channel);
		static std::string RPL_TOPIC(const std::string& server, const std::string& channel, const std::string& topic);
		static std::string RPL_NAMREPLY(const std::string& server, const std::string& channel, const std::string& names);
		static std::string RPL_ENDOFNAMES(const std::string& server, const std::string& channel);

		// === Commandes utilisateurs ===

		static std::string RPL_JOIN(const std::string& prefix, const std::string& channel);
		static std::string RPL_PART(const std::string& prefix, const std::string& channel, const std::string& reason);
		static std::string RPL_PRIVMSG(const std::string& prefix, const std::string& target, const std::string& message);
		static std::string RPL_QUIT(const std::string& prefix, const std::string& reason);
		static std::string RPL_MODE(const std::string& prefix, const std::string& target, const std::string& modes);

		// === Erreurs générales ===

		static std::string ERR_NOSUCHNICK(const std::string& server, const std::string& target);
		static std::string ERR_NOSUCHCHANNEL(const std::string& server, const std::string& channel);
		static std::string ERR_CANNOTSENDTOCHAN(const std::string& server, const std::string& channel);
		static std::string ERR_NOTEXTTOSEND(const std::string& server);
		static std::string ERR_UNKNOWNCOMMAND(const std::string& server, const std::string& command);
		static std::string ERR_NONICKNAMEGIVEN(const std::string& server);
		static std::string ERR_ERRONEUSNICKNAME(const std::string& server, const std::string& badnick);
		static std::string ERR_NICKNAMEINUSE(const std::string& server, const std::string& badnick);
		static std::string ERR_USERNOTINCHANNEL(const std::string& server, const std::string& user, const std::string& channel);
		static std::string ERR_NOTONCHANNEL(const std::string& server, const std::string& channel);
		static std::string ERR_NOTREGISTERED(const std::string& server);
		static std::string ERR_NEEDMOREPARAMS(const std::string& server, const std::string& command);
		static std::string ERR_ALREADYREGISTERED(const std::string& server);
		static std::string ERR_PASSWDMISMATCH(const std::string& server);
		
		static std::string ERR_KEYSET(const std::string& server, const std::string& channel);
		static std::string ERR_UNKNOWNMODE(const std::string& server, const std::string& mode);

		static std::string ERR_BADCHANNELKEY(const std::string& server, const std::string& channel);
		static std::string ERR_CHANOPRIVSNEEDED(const std::string& server, const std::string& channel);
		
		static std::string ERR_UMODEUNKNOWNFLAG(const std::string& server);
		static std::string ERR_USERSDONTMATCH(const std::string& server);

		// === External Use Methods ===
		static bool		checkClientRights(Server &serv, std::string command, int it);
		static void		sendError(Server &serv, int error, int it, std::string opt1, std::string opt2);
		static void		sendReply(Server &serv, int reply, int it, std::string opt1, std::string opt2);
		static void		welcomeClient(Server &serv, int it);
		
	private:
		static std::string format(const std::string& server, const std::string& code, const std::string& nick, const std::string& message);
};
#endif
