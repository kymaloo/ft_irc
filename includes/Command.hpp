#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>

class Command
{
    private:
        std::string _input;       // Ligne brute reçue
        std::string _prefix;      // Préfixe facultatif (ex: nick!user@host)
        std::string _name;        // Nom de commande (ex: PRIVMSG, JOIN, QUIT)
        std::vector<std::string> _params; // Liste de paramètres
        bool _valid;              // Indique si la commande est syntaxiquement valide

    private:
        void removeCRLF(std::string& str);
        void parsePrefix(std::stringstream& ss, std::string& str);
        void parseCommand(std::stringstream& ss);
        void parseParams(std::stringstream& ss);

    public:
        Command(const std::string& input);
        void parse();

        // Getters
        const std::string& getPrefix() const { return _prefix; }
        const std::string& getName() const { return _name; }
        const std::vector<std::string>& getParams() const { return _params; }
        bool isValid() const { return _valid; }
};

#endif
