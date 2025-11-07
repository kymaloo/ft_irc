#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>

class Channel
{
	private:
		std::string			_name;
		std::map<int, bool>	_fdClient;
	private:
		void addClient(const int &fd);
	public:
		Channel();
		Channel(const std::string &name, const int &fd);
		~Channel();

		std::string getName();
};

#endif
