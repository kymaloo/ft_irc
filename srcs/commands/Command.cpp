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

    _valid = !_name.empty();
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
    if (!(ss >> _name))
        return ; 
    std::transform(_name.begin(), _name.end(), _name.begin(), ::toupper);
}

//  Extrait les paramètres
void Command::parseParams(std::stringstream& ss)
{
    std::string param;

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
            _params.push_back(param);
        }
    }
}

void Command::setInput(std::string &input)
{
    this->_input = input;
}

void Command::redirectionCommand()
{
    switch (this->_name[0])
    {
        case 'J':
            if (this->_name == "JOIN")
            {
                std::string channelName = this->_params[0];
                // join(channelName, client);
            }
            break;
        default:
            break;
    }
}