#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>

class Command
{
	private:

	public:
		Command();
		Command(const Command &cpy);
		Command &operator=(const Command &ref);
		~Command();
};

#endif