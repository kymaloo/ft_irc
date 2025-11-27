#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <poll.h>
# include <iostream>
# include <iterator>
# include <sstream>
# include <map>

# include "reply.hpp"
# include "forward.hpp"

class Command
{
	private:
		std::string					_input;			// Ligne brute reçue
		std::string					_prefix;		// Préfixe facultatif (ex: nick!user@host)
		std::string					_commandName;	// Nom de commande (ex: PRIVMSG, JOIN, QUIT)
		std::vector<std::string>	_params;		// Liste de paramètres
		bool						_valid;			// Indique si la commande est syntaxiquement valide
		//struct pollfd _pfds[200];

	// Parser

		void	removeCRLF(std::string& str);
		void	parsePrefix(std::stringstream& ss, std::string& str);
		void	parseCommand(std::stringstream& ss);
		void	parseParams(std::stringstream& ss);
		bool	checkDoublon(std::string &param);
		void	clearParams();
	// Commandes

		void	quit(Server &serv, int fdClient);
		void	pass(Server &serv, int fdClient);
		void	nick(Server &serv, int fdClient);
		void	user(Server &serv, int fdClient);
		void	privmsg(Server &serv, int fdClient);
		void	join(Server &serv, int fdClient);
		void 	part(Server &serv, int fdClient);
		void	topic(Server& serv, int fdClient);
		void	mode(Server& serv, int fdClient);
		void	kick(Server &serv, int fdClient);

		bool	isNameChannelValid(Server &serv, std::string &channel, int itClient);
		bool	checkNumberParam(Server &serv, int itClient);
		void	checkEntryChannel(Server &serv, int it);
		bool	isChannelIntoList(Server &serv, std::string &vecChannel);
		bool	isMdpValid(Server &serv, std::string &channel, int itClient);
		bool	checkChannelforKick(Server &serv, int fdClient);
		bool	checkUserForKick(Server &serv, int fdClient);
		size_t	getIteratorChannel(Server &serv, std::string &vecChannel);

	public:
		Command(const std::string& input);
		Command();
		Command	&operator=(const Command &cpy);
		bool	parse();

		// Getters

		const std::string&				getPrefix() const	{ return _prefix; }
		const std::string&				getName() const		{ return _commandName; }
		const std::vector<std::string>&	getParams() const	{ return _params; }
		const std::string&				getInput() const	{ return _input; }
		bool							isValid() const		{ return _valid; }

		// Setters

		void	setInput(std::string &input);
		void	redirectionCommand(Server &serv, int it);
		void	multiCommands(Server &serv, int it);
};

std::vector<std::string> split(std::string &str);

#endif
