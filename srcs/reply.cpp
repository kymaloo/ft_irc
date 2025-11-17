#include "../includes/reply.hpp"

// Format de base commun
std::string Reply::format(const std::string& server, const std::string& code, const std::string& nick, const std::string& message) {
    return ":" + server + " " + code + " " + nick + " " + message + "\r\n";
}




// === Réponses de connexion ===
std::string Reply::RPL_WELCOME(const std::string& server, const std::string& nick, const std::string& user, const std::string& host) {
    return format(server, "001", nick, ":Welcome to the Internet Relay Network, " + nick + "!" + user + "@" + host);
}

std::string Reply::RPL_YOURHOST(const std::string& server, const std::string& nick, const std::string& version) {
    return format(server, "002", nick, ":Your host is " + server + ", running version " + version);
}

std::string Reply::RPL_CREATED(const std::string& server, const std::string& nick, const std::string& date) {
    return format(server, "003", nick, ":This server was created " + date);
}

std::string Reply::RPL_MYINFO(const std::string& server, const std::string& nick, const std::string& version, const std::string& userModes, const std::string& channelModes) {
    return format(server, "004", nick, server + " " + version + " " + userModes + " " + channelModes);
}




// === Commandes de canaux ===
std::string Reply::RPL_NAMREPLY(const std::string& server, const std::string& nick, const std::string& channel, const std::string& names) {
    return format(server, "353", nick, "= " + channel + " :" + names);
}

std::string Reply::RPL_ENDOFNAMES(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "366", nick, channel + " :End of /NAMES list");
}

std::string Reply::RPL_TOPIC(const std::string& server, const std::string& nick, const std::string& channel, const std::string& topic) {
    return format(server, "332", nick, channel + " :" + topic);
}




// === Commandes utilisateurs ===
std::string Reply::RPL_JOIN(const std::string& prefix, const std::string& channel) {
    return ":" + prefix + " JOIN :" + channel + "\r\n";
}

std::string Reply::RPL_PART(const std::string& prefix, const std::string& channel, const std::string& reason) {
    return ":" + prefix + " PART " + channel + " :" + reason + "\r\n";
}

std::string Reply::RPL_PRIVMSG(const std::string& prefix, const std::string& target, const std::string& message) {
    return ":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n";
}

std::string Reply::RPL_QUIT(const std::string& prefix, const std::string& reason) {
    return ":" + prefix + " QUIT :" + reason + "\r\n";
}

std::string Reply::RPL_MODE(const std::string& prefix, const std::string& target, const std::string& modes) {
    return ":" + prefix + " MODE " + target + " " + modes + "\r\n";
}




// === Erreurs ===
std::string Reply::ERR_NOSUCHNICK(const std::string& server, const std::string& nick, const std::string& target) {
    return format(server, "401", nick, target + " :No such nick/channel");
}

std::string Reply::ERR_NOSUCHCHANNEL(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "403", nick, channel + " :No such channel");
}

std::string Reply::ERR_CANNOTSENDTOCHAN(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "404", nick, channel + " :Cannot send to channel");
}

std::string Reply::ERR_NICKNAMEINUSE(const std::string& server, const std::string& nick, const std::string& badnick) {
    return format(server, "433", nick, badnick + " :Nickname is already in use");
}

std::string Reply::ERR_NOTREGISTERED(const std::string& server, const std::string& nick) {
    return format(server, "451", nick, ":You have not registered");
}

std::string Reply::ERR_NEEDMOREPARAMS(const std::string& server, const std::string& nick, const std::string& command) {
    return format(server, "461", nick, command + " :Not enough parameters");
}

std::string Reply::ERR_ALREADYREGISTERED(const std::string& server, const std::string& nick) {
    return format(server, "462", nick, ":You may not reregister");
}

std::string Reply::ERR_UNKNOWNCOMMAND(const std::string& server, const std::string& nick, const std::string& command) {
    return format(server, "421", nick, command + " :Unknown command");
}

std::string Reply::ERR_NOTONCHANNEL(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "442", nick, channel + " :You're not on that channel");
}

std::string Reply::ERR_USERNOTINCHANNEL(const std::string& server, const std::string& nick, const std::string& user, const std::string& channel) {
    return format(server, "441", nick, user + " " + channel + " :They aren't on that channel");
}

std::string Reply::ERR_CHANOPRIVSNEEDED(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "482", nick, channel + " :You're not channel operator");
}

std::string Reply::ERR_PASSWDMISMATCH(const std::string& server) {
    return format(server, "464", ":password ", "incorrect");
}

std::string Reply::ERR_BADCHANNELKEY(const std::string& server, const std::string& channel) {
    return format(server, "475", channel, ":Cannot join channel (+k)");
}

void Reply::welcomeClient(Server &serv, int it)
{
	std::string	message;
    char buffer[1024];

	serv.setClientRegister(it, true);

	message = Reply::RPL_WELCOME(serv.getServName(), serv.getClientNick(it), serv.getClientUser(it), inet_ntop(AF_INET, &(serv.getSockAddr().sin_addr), buffer, 1024));
	send(serv.getClientfd(it), message.c_str(), message.size(), 0);

	message = Reply::Reply::RPL_YOURHOST(serv.getServName(), serv.getClientNick(it), "version");
	send(serv.getClientfd(it), message.c_str(), message.size(), 0);

	message = Reply::RPL_CREATED(serv.getServName(), serv.getClientNick(it), "date");
	send(serv.getClientfd(it), message.c_str(), message.size(), 0);

	message = Reply::RPL_MYINFO(serv.getServName(), serv.getClientNick(it), "version", "userModes", "channelModes");
	send(serv.getClientfd(it), message.c_str(), message.size(), 0);
}

void Reply::sendError(Server &serv, int error, int it)
{
	std::string	message;
	
	switch(error)
	{
		case 401 :
			message = Reply::ERR_NOSUCHNICK(serv.getServName(), serv.getClientNick(it), "target");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 403 :
			message = Reply::ERR_NOSUCHCHANNEL(serv.getServName(), serv.getClientNick(it), "channel");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 404 :
			message = Reply::ERR_CANNOTSENDTOCHAN(serv.getServName(), serv.getClientNick(it), "channel");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 433 :
			message = Reply::ERR_NICKNAMEINUSE(serv.getServName(), serv.getClientNick(it), "badnick");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 451 :
			message = Reply::ERR_NOTREGISTERED(serv.getServName(), serv.getClientNick(it));
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 461 :
			message = Reply::ERR_NEEDMOREPARAMS(serv.getServName(), serv.getClientNick(it), "command");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 462 :
			message = Reply::ERR_ALREADYREGISTERED(serv.getServName(), serv.getClientNick(it));
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 421 :
			message = Reply::ERR_UNKNOWNCOMMAND(serv.getServName(), serv.getClientNick(it), "command");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 442 :
			message = Reply::ERR_NOTONCHANNEL(serv.getServName(), serv.getClientNick(it), "channel");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 441 :
			message = Reply::ERR_USERNOTINCHANNEL(serv.getServName(), serv.getClientNick(it), "user", "channel");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
			return ;
		case 482 :
			message = Reply::ERR_CHANOPRIVSNEEDED(serv.getServName(), serv.getClientNick(it), "channel");
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
		case 464 :
			message = Reply::ERR_PASSWDMISMATCH(serv.getServName());
			send(serv.getClientfd(it), message.c_str(), message.size(), 0);
	}
}