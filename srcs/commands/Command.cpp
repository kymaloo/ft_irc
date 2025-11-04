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

//  Extrait les paramètres
void Command::parseParams(std::stringstream& ss)
{
    std::string param ;
    // int i = 0;
    // if (ss.str()[i] == ':')
    //     i++;

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
            param.erase(std::remove(param.begin(), param.end(), ','), param.end());
            _params.push_back(param);
        }
    }
}

void Command::setInput(std::string &input)
{
    this->_input = input;
}

void Command::setNameServ(std::string &input)
{
    this->_serverName = input;
}

void Command::setPfds(pollfd *pfds)
{
    for (int i = 0; i < 200; i++)
        _pfds[i] = pfds[i];
}

void Command::redirectionCommand()
{
    parse();
    
    switch (this->_commandName[0])
    {
        case 'J':
            if (this->_commandName == "JOIN")
            {
                std::string channelName = this->_params[0];
                join(_serverName, _params[0], channelName);
            }
            break;
        default:
            break;
    }
    _pfds[1].revents = 0;
}
