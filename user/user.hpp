#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User
{
	private:
		bool	isOp;
		bool	inChannel;
	public:
		User();
		User(const User &cpy);
		User &operator=(const User &ref);
		~User();
};

#endif