#include "../includes/reply.hpp"
#include "../includes/forward.hpp"

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

//?221 - RPL_UMODEIS # a voir si besoin
// static std::string RPL_UMODEIS(const std::string& server, const std::string& nick, const std::string& channel){
// 	return format(server, "221", nick, )
// }
//?324 - RPL_CHANNELMODEIS # a voir si besoin
// static std::string RPL_CHANNELMODEIS(const std::string& server, const std::string& nick, const std::string& channel){

// }
//331 - RPL_NOTOPIC
std::string Reply::RPL_NOTOPIC(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "331", nick, channel + " :" + "No topic is set");
}
//332 - RPL_TOPIC
std::string Reply::RPL_TOPIC(const std::string& server, const std::string& nick, const std::string& channel, const std::string& topic) {
    return format(server, "332", nick, channel + " :" + topic);
}
//353 - RPL_NAMREPLY
std::string Reply::RPL_NAMREPLY(const std::string& server, const std::string& nick, const std::string& channel, const std::string& names) {
    return format(server, "353", nick, "= " + channel + " :" + names);
}
//366 - RPL_ENDOFNAMES
std::string Reply::RPL_ENDOFNAMES(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "366", nick, channel + " :End of /NAMES list");
}


// 

// === Commandes utilisateurs ===

//RPL_JOIN
std::string Reply::RPL_JOIN(const std::string& prefix, const std::string& channel) {
    return ":" + prefix + " JOIN :" + channel + "\r\n";
}
//RPL_PART
std::string Reply::RPL_PART(const std::string& prefix, const std::string& channel, const std::string& reason) {
    return ":" + prefix + " PART " + channel + " :" + reason + "\r\n";
}
//RPL_PRIVMSG
std::string Reply::RPL_PRIVMSG(const std::string& prefix, const std::string& target, const std::string& message) {
    return ":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n";
}
//RPL_QUIT
std::string Reply::RPL_QUIT(const std::string& prefix, const std::string& reason) {
    return ":" + prefix + " QUIT :" + reason + "\r\n";
}
//RPL_MODE
std::string Reply::RPL_MODE(const std::string& prefix, const std::string& target, const std::string& modes) {
    return ":" + prefix + " MODE " + target + " " + modes + "\r\n";
}




// === Erreurs ===

//401 - ERR_NOSUCHNICK
std::string Reply::ERR_NOSUCHNICK(const std::string& server, const std::string& nick, const std::string& target) {
    return format(server, "401", nick, target + " :No such nick/channel");
}

//403 - ERR_NOSUCHCHANNEL
std::string Reply::ERR_NOSUCHCHANNEL(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "403", nick, channel + " :No such channel");
}

//404 - ERR_CANNOTSENDTOCHAN
std::string Reply::ERR_CANNOTSENDTOCHAN(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "404", nick, channel + " :Cannot send to channel");
}

//412 -  ERR_NOTEXTTOSEND
std::string Reply::ERR_NOTEXTTOSEND(const std::string& server, const std::string& nick) {
	return format(server, "412", nick, ":No text to send");
}

//421 - ERR_UNKNOWNCOMMAND
std::string Reply::ERR_UNKNOWNCOMMAND(const std::string& server, const std::string& nick, const std::string& command) {
    return format(server, "421", nick, command + " :Unknown command");
}

//431 - ERR_NONICKNAMEGIVEN
std::string Reply::ERR_NONICKNAMEGIVEN(const std::string& server, const std::string& nick) {
	return format(server, "431", nick, ":No nickname given");
}

//432 - ERR_ERRONEUSNICKNAME
std::string Reply::ERR_ERRONEUSNICKNAME(const std::string& server, const std::string& nick, const std::string& badnick) {
	return format(server, "432", nick, badnick + " :Erroneous nickname");
}

//433 - ERR_NICKNAMEINUSE
std::string Reply::ERR_NICKNAMEINUSE(const std::string& server, const std::string& nick, const std::string& badnick) {
    return format(server, "433", nick, badnick + " :Nickname is already in use");
}

//441 - ERR_USERNOTINCHANNEL
std::string Reply::ERR_USERNOTINCHANNEL(const std::string& server, const std::string& nick, const std::string& user, const std::string& channel) {
    return format(server, "441", nick, user + " " + channel + " :They aren't on that channel");
}

//442 - ERR_NOTONCHANNEL
std::string Reply::ERR_NOTONCHANNEL(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "442", nick, channel + " :You're not on that channel");
}

//451 - ERR_NOTREGISTERED
std::string Reply::ERR_NOTREGISTERED(const std::string& server, const std::string& nick) {
    return format(server, "451", nick, ":You have not registered");
}

//461 - ERR_NEEDMOREPARAMS
std::string Reply::ERR_NEEDMOREPARAMS(const std::string& server, const std::string& nick, const std::string& command) {
    return format(server, "461", nick, command + " :Not enough parameters");
}

//462 - ERR_ALREADYREGISTERED
std::string Reply::ERR_ALREADYREGISTERED(const std::string& server, const std::string& nick) {
    return format(server, "462", nick, ":You may not reregister");
}

//464 - ERR_PASSWDMISMATCH
std::string Reply::ERR_PASSWDMISMATCH(const std::string& server) {
    return format(server, "464", ":password ", "incorrect");
}

//467 - ERR_KEYSET
std::string Reply::ERR_KEYSET(const std::string& server, const std::string& channel){
	return format(server, "467", channel, ":Channel key already set");
}

//472 - ERR_UNKNOWNMODE
std::string Reply::ERR_UNKNOWNMODE(const std::string& server, const std::string& mode){
	return format(server, "467", mode, ":is unknown mode char to me");
}

//475 - ERR_BADCHANNELKEY
std::string Reply::ERR_BADCHANNELKEY(const std::string& server, const std::string& channel) {
    return format(server, "475", channel, ":Cannot join channel (+k)");
}

//482 - ERR_CHANOPRIVSNEEDED
std::string Reply::ERR_CHANOPRIVSNEEDED(const std::string& server, const std::string& nick, const std::string& channel) {
    return format(server, "482", nick, channel + " :You're not channel operator");
}

//501 - ERR_UMODEUNKNOWNFLAG
std::string Reply::ERR_UMODEUNKNOWNFLAG(const std::string& server){

    return format(server, "501", ":Unknown ", "MODE flag");
}

//502 - ERR_USERSDONTMATCH
std::string Reply::ERR_USERSDONTMATCH(const std::string& server){

    return format(server, "502", ":Cant ", "change mode for other users");
}


#include "../includes/server.hpp"

//Check the client's rights to execute the said command
bool Reply::checkClientRights(Server &serv, std::string command, int itClient)
{
	if (serv.didClientPass(itClient) == false)
	{
		if (command != "PASS")
			return false;
	}
	else if (serv.didClientRegister(itClient) == false)
	{
		if (command != "NICK" && command != "USER" && command != "PASS")
		{
			Reply::sendError(serv, 451, itClient, "NULL", "NULL");
			return false;
		}
	}
	return true;
}

void Reply::welcomeClient(Server &serv, int itClient)
{
	std::string	message;
    char buffer[1024];

	serv.setClientRegister(itClient, true);

	message = Reply::RPL_WELCOME(serv.getServName(), serv.getClientNick(itClient), serv.getClientUser(itClient), inet_ntop(AF_INET, &(serv.getSockAddr().sin_addr), buffer, 1024));
	send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);

	message = Reply::Reply::RPL_YOURHOST(serv.getServName(), serv.getClientNick(itClient), "'version'");
	send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);

	message = Reply::RPL_CREATED(serv.getServName(), serv.getClientNick(itClient), "'date'");
	send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);

	message = Reply::RPL_MYINFO(serv.getServName(), serv.getClientNick(itClient), "'version'", "'userModes'", "'channelModes'");
	send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
}

void Reply::sendError(Server &serv, int error, int itClient, std::string opt1, std::string opt2)
{
	std::string	message;

	(void)opt1;
	(void)opt2;
	switch (error)
	{
		case 401 :
			message = Reply::ERR_NOSUCHNICK(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 403 :
			message = Reply::ERR_NOSUCHCHANNEL(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 404 :
			message = Reply::ERR_CANNOTSENDTOCHAN(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 412 :
			message = Reply::ERR_NOTEXTTOSEND(serv.getServName(), serv.getClientNick(itClient));
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 421 :
			message = Reply::ERR_UNKNOWNCOMMAND(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 431 :
			message = Reply::ERR_NONICKNAMEGIVEN(serv.getServName(), serv.getClientNick(itClient));
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 432 :
			message = Reply::ERR_ERRONEUSNICKNAME(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 433 :
			message = Reply::ERR_NICKNAMEINUSE(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 441 :
			message = Reply::ERR_USERNOTINCHANNEL(serv.getServName(), serv.getClientNick(itClient), opt1, opt2);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 442 :
			message = Reply::ERR_NOTONCHANNEL(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 451 :
			message = Reply::ERR_NOTREGISTERED(serv.getServName(), serv.getClientNick(itClient));
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 461 :
			message = Reply::ERR_NEEDMOREPARAMS(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 462 :
			message = Reply::ERR_ALREADYREGISTERED(serv.getServName(), serv.getClientNick(itClient));
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 464 :
			message = Reply::ERR_PASSWDMISMATCH(serv.getServName());
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return;
		case 467 :
			message = Reply::ERR_KEYSET(serv.getServName(), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 472 :
			message = Reply::ERR_UNKNOWNMODE(serv.getServName(), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return;
		case 482 :
			message = Reply::ERR_CHANOPRIVSNEEDED(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 501:
			message = Reply::ERR_UMODEUNKNOWNFLAG(serv.getServName());
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return;
		case 502:
			message = Reply::ERR_USERSDONTMATCH(serv.getServName());
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return;
	}
}

void Reply::sendReply(Server &serv, int reply, int itClient, std::string opt1, std::string opt2)
{
	std::string	message;

	(void)opt1;
	(void)opt2;
	switch (reply)
	{
		// case 221:
		// 	//TODO RPL_UMODEIS (if needed)
		// 	return ;
		// case 324:
		// 	//TODO RPL_CHANNELMODEIS (if needed)
		// 	return ;
		case 331:
			message = Reply::RPL_NOTOPIC(serv.getServName(), serv.getClientNick(itClient), opt1);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
		case 332:
			message = Reply::RPL_TOPIC(serv.getServName(), serv.getClientNick(itClient), opt1, opt2);
			send(serv.getClientfd(itClient), message.c_str(), message.size(), 0);
			return ;
	}
}
