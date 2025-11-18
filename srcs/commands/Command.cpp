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
void Command::parse()
{
    std::string str = _input;

    removeCRLF(str);
    std::stringstream ss(str);

    parsePrefix(ss, str);
    parseCommand(ss);
    parseParams(ss);

    _valid = !_commandName.empty();
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
    {
        // std::cout << "size: " << _params.size() << std::endl;
        _params.pop_back();
    }
}

void Command::setInput(std::string &input)
{
    this->_input = input;
}

// void Command::setNameServ(std::string &input)
// {
//     this->_serverName = input;
// }

// void Command::setPfds(pollfd *pfds)
// {
//     for (int i = 0; i < 200; i++)
//         _pfds[i] = pfds[i];
// }

void Command::multiCommands(Server &serv, int it)
{
	std::string	fullInput = _input;
	size_t prevPos = 0;
	size_t pos = fullInput.find("\n");
	std::string	line;

	while (pos != std::string::npos)
	{
		line = fullInput.substr(prevPos, pos);

        if (line == "\r\n" || line.empty())
        {
            prevPos = pos + 1;
            pos = fullInput.find("\n", prevPos);
            continue ;
        }
        setInput(line);
		redirectionCommand(serv, it);

		prevPos = pos + 1;
		pos = fullInput.find("\n", prevPos);
	}
}

void Command::redirectionCommand(Server &serv, int it)
{
	parse();
	if (!_valid || _commandName.empty())
	{
		std::cout << "Invalid command\n";
		return;
	}
	switch (this->_commandName[0])
	{
		case 'J':
			if (this->_commandName == "JOIN")
			{
				std::string Nick = "Nick : Kymaloo";
				if (!_params.empty())
					join(serv, Nick, it);
			}
			break;
		case 'P':
			if (this->_commandName == "PRIVMSG")
			{
				if (!_params.empty())
                    privmsg(serv, it);
			}
            else if (this->_commandName == "PART")
            {
                if (!_params.empty())
                       part(serv, it);
            }
		// 	break;
		case 'N':
			if (this->_commandName == "NICK")
			{
				if (!_params.empty())
                    nick(serv, it);	
				else
					std::cout << "No param for NICK\n";		
			}
			break;
		case 'U':
			if (this->_commandName == "USER")
			{
				if (!_params.empty())
                    user(serv, it);	
				else
					std::cout << "No param for USER\n";		
			}
			break;
		default:
			break;
	}
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
