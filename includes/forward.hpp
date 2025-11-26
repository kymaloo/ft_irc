#ifndef FORWARD_HPP
# define FORWARD_HPP

//	###	COLORS	###
#define GREY        "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define WHITE       "\033[0;37m"

class Server;
class Command;
class Channel;
class Client;

# include <iostream>
# include <sstream>
# include <vector>

class ToolBox
{
	public:
		/*Returns true if the whole string is digital.
		Does not check if it fits in an int.
		Can start with + or -.*/
		static bool	isNum(std::string str)
		{
			int i = 0;
			if (str[0] == '-' || str[0] == '+')
				i++;
			while (str[i])
			{
				if (std::isdigit(str[i]) == false)
					return false;
				i++;
			}
			return true;
		}

		/*Returns a vector of strings containing individual elements separated by <delim>.*/
		static std::vector<std::string> split(std::string &str, char delim)
		{
			std::vector<std::string> result;
			std::stringstream ss(str);
			std::string token;

			if (str.find(delim) == std::string::npos)
			{
				result.push_back(str);
				return result;
			}
			while (std::getline(ss, token, delim))
				result.push_back(token);
			return result;
		}
};

#endif