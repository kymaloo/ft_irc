#include "../../includes/server.hpp"
#include "../../includes/Command.hpp"
#include <sstream>
#include <algorithm>

// === Constructeur ===
Command::Command(const std::string& input) : _input(input), _valid(false)
{

}

Command::Command()
{
	this->_valid = false;
}

Command &Command::operator=(const Command &cpy)
{
	if (!cpy._input.empty())
		this->_input = cpy._input;
	if (!cpy._prefix.empty())
		this->_prefix = cpy._prefix;
	if (!cpy._commandName.empty())
		this->_commandName = cpy._commandName;
	if (!cpy._params.empty())
		this->_params = cpy._params;
	this->_valid = cpy._valid;
	return (*this);
}

// === Fonction principale ===
bool Command::parse()
{
	std::string str = _input;

	removeCRLF(str);
	std::stringstream ss(str);

	parsePrefix(ss, str);
	parseCommand(ss);
	parseParams(ss);

	_valid = !_commandName.empty();
	return _valid;
}

//  Supprime les caractères CRLF de fin
void Command::removeCRLF(std::string& str)
{
	if (str.size() >= 2 && str[str.size() - 2] == '\r' && str[str.size() - 1] == '\n')
		str = str.substr(0, str.size() - 2);
}

//  Extrait le préfixe (facultatif)
void Command::parsePrefix(std::stringstream& ss, std::string& str)
{
	if (!str.empty() && str[0] == ':')
	{
		std::string token;
		ss >> token;
		_prefix = token.substr(1);
	}
}

//  Extrait le nom de la commande
void Command::parseCommand(std::stringstream& ss)
{
	if (!(ss >> _commandName))
		return ;
	std::transform(_commandName.begin(), _commandName.end(), _commandName.begin(), ::toupper);
}

bool Command::checkDoublon(std::string &param)
{
	for (size_t i = 0; i != _params.size(); i++)
	{
		if (param == _params[i])
			return (true);
	}
	return (false);
}

//  Extrait les paramètres
void Command::parseParams(std::stringstream& ss)
{
	std::string param;
	int i = 0;
	if (_params.size() != 0)
		clearParams();
	while (ss.good())
	{
		char c = ss.peek();
		if (c == ' ')
		{  
			ss.get();
			continue;
		}
		if (c == ':')
		{
			ss.get();
			std::getline(ss, param);
			_params.push_back(param);
			break;
		}
		else
		{
			ss >> param;
			if (checkDoublon(param) == true)
				continue;
			_params.push_back(param);
		}
		i++;
	}
}

void Command::clearParams()
{
	while (_params.size() != 0)
		_params.pop_back();
}

void Command::setInput(std::string &input)
{
	this->_input = input;
}

void Command::multiCommands(Server &serv, int fdClient)
{
	std::string	fullInput = _input;
	size_t prevPos = 0;
	size_t pos = fullInput.find("\n");
	std::string	line;

	while (pos != std::string::npos)
	{
		line = fullInput.substr(prevPos, pos);

		if (line == "\n" || line.empty())
		{
			prevPos = pos + 1;
			pos = fullInput.find("\n", prevPos);
			continue ;
		}
		setInput(line);
		redirectionCommand(serv, fdClient);

		prevPos = pos + 1;
		pos = fullInput.find("\n", prevPos);
	}
}

void Command::redirectionCommand(Server &serv, int fdClient)
{
	if (parse() == false)
	{
		std::cout << "Invalid command\n";
		return;
	}
	if (Reply::checkClientRights(serv, _commandName, fdClient) == false)
	{
		if (_params.empty() == false)
			clearParams();
		return ;
	}
	switch (this->_commandName[0])
	{
		case 'C':
			if (this->_commandName == "CAP LS")
				Reply::capls(serv, fdClient);
		case 'I':
			if (this->_commandName == "INVITE")
				invite(serv, fdClient);
		case 'J':
			if (this->_commandName == "JOIN")
			{
				if (!_params.empty())
					join(serv, fdClient);
			}
			break;
		case 'K':
			if (this->_commandName == "KICK")
				kick(serv, fdClient);
			break;
		case 'L':
			if (this->_commandName == "LIST")
				printChannel(serv);
			break;
		case 'M':
			if (this->_commandName == "MODE")
				mode(serv, fdClient);
			break;
		case 'N':
			if (this->_commandName == "NICK")
				nick(serv, fdClient);
			break;
		case 'P':
			if (this->_commandName == "PART" && !_params.empty())
				part(serv, fdClient);
			else if (this->_commandName == "PASS")
				pass(serv, fdClient);
			else if (this->_commandName == "PING")
				Reply::pong(serv, fdClient);
			else if (this->_commandName == "PRIVMSG")
				privmsg(serv, fdClient);
			break;
		case 'Q':
			if (this->_commandName == "QUIT")
				quit(serv, fdClient);
			break;
		case 'T':
			if (this->_commandName == "TOPIC")
				topic(serv, fdClient);
			break;
		case 'U':
			if (this->_commandName == "USER")
				user(serv, fdClient);
			break;
	}
	if (_params.empty() == false)
		clearParams();
}

std::vector<std::string> split(std::string &str)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string token;

	if (str.find(',') == std::string::npos)
	{
		result.push_back(str);
		return result;
	}
	while (std::getline(ss, token, ','))
		result.push_back(token);
	return result;
}
