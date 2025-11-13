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
    std::cout << "C'est moi le input " << _input << std::endl;

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
        std::cout << i << std::endl;
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
        std::cout << "size: " << _params.size() << std::endl;
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
    if (!_valid || _commandName.empty())
        return;
    if (isValid())
	{
	// 	std::cout << "Prefix: " << getPrefix() << std::endl;
	// 	std::cout << "Commande: " << getName() << std::endl;
		std::cout << "Params:" << std::endl;
		for (size_t i = 0; i < _params.size(); ++i)
			std::cout << "  [" << i << "]: " << _params[i] << std::endl;
	}
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
        default:
            break;
    }
}

std::vector<std::string> split(std::string str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, ','))
        result.push_back(token);
    return result;
}
