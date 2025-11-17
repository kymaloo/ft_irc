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
	if (cpy.getInput().empty() == false)
		this->_input = cpy._input;
	if (cpy.getPrefix().empty() == false)
		this->_prefix = cpy._prefix;
	if (cpy.getName().empty() == false)
		this->_commandName = cpy._commandName;
	if (cpy.getParams().empty() == false)
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
            //param.erase(std::remove(param.begin(), param.end(), ','), param.end());
            // si doublon continue
            if (checkDoublon(param) == true)
                continue;
            _params.push_back(param);
        }
        i++;
    }
}

void Command::clearParams()
{
    // std::cout << "Before :\n";
    // for (size_t i = 0; i < _params.size(); ++i)
	// 	std::cout << "  [" << i << "]: " << _params[i] << std::endl;
    while (_params.size() != 0)
    {
        // std::cout << "size: " << _params.size() << std::endl;
        _params.pop_back();
    }
    // std::cout << "After:\n";
    // for (size_t i = 0; i < _params.size(); ++i)
	// 	std::cout << "  [" << i << "]: " << _params[i] << std::endl;
}

void Command::setInput(std::string &input)
{
   // this->_input.clear();
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

void Command::redirectionCommand(Server &serv, int it)
{
	parse();
	std::cout << "Command parsed: " << _commandName << std::endl;
	if (!_valid || _commandName.empty())
	{
		std::cout << "Invalid command\n";		
		return;
	}
	std::cout << "Redirecting command: " << _commandName << std::endl;
	switch (this->_commandName[0])
	{
		case 'J':
			if (this->_commandName == "JOIN")
			{
				std::string Nick = "Nick : Kymaloo";
				//std::cout << _params[0];
				if (!_params.empty())
					join(serv, Nick, it);
			}
			break;
		case 'P':
			if (this->_commandName == "PRIVMSG")
			{
				std::string Nick = "Nick : Kymaloo";
				// std::cout << _params[0];
				if (!_params.empty())
					privmsg(serv, Nick, _params[0], it);
			}
			break;
		case 'N':
			if (this->_commandName == "NICK")
			{
				std::string Nick = "Nick : Kymaloo";
				// std::cout << _params[0];
				if (!_params.empty())
					nick(serv, it);	
				else
					std::cout << "No param for NICK\n";		
			}
			break;
		default:
			break;
	}
	//_input.clear();
}

int countWord(std::string str)
{
	std::istringstream myStream(str);
	std::string token;

    size_t pos = -1;
	int count = 0;

    while (myStream >> token)
	{
        while ((pos = token.rfind(',')) != std::string::npos)
            token.erase(pos, 1);
		count++;
	}
	return (count);
}

std::vector<std::string> split(std::string &str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, ','))
        result.push_back(token);
    return result;
}
