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
	private:
		void	removeCRLF(std::string& str);
		void	parsePrefix(std::stringstream& ss, std::string& str);
		void	parseCommand(std::stringstream& ss);
		void	parseParams(std::stringstream& ss);
    bool  checkDoublon(std::string &param);
		void	clearParams();
	// Commandes
	private:
		std::string	nick(Server &serv, int iterator);
		std::string	user(Server &serv, int iterator);
		std::string	privmsg(Server &serv, int iterator);
		void		join(Server &serv, std::string &nick, int it);
		bool		isNameChannelValid(Server &serv, std::string &nick, std::string &channel, int it);
		bool    checkNumberParam(Server &serv, std::string &nick, int it);
		void    checkEntryChannel(Server &serv, std::string &nick, int it);
		bool    isChannelInToList(Server &serv, std::vector<std::string> &vecChannel, std::string &nick, int it);
    bool isMdpValid(Server &serv, std::string &channel, int it);
    size_t getIteratorChannel(Server &serv, std::string &vecChannel);
	// Setter
	public:
		Command(const std::string& input);
		Command();
		Command &operator=(const Command &cpy);
		void parse();


		// Getters
		const	std::string& getPrefix() const { return _prefix; }
		const	std::string& getName() const { return _commandName; }
		const	std::vector<std::string>& getParams() const { return _params; }
		const	std::string& getInput() const { return _input; }
		bool	isValid() const { return _valid; }

		
		void	setInput(std::string &input);
		void	redirectionCommand(Server &serv, int it);
		void	multiCommands(Server &serv, int it);
};
std::vector<std::string> split(std::string &str);

#endif
