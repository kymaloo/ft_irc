NAME = irc

SRCS =	main.cpp						\
		srcs/server.cpp					\
		srcs/client.cpp					\
		srcs/commands/Command.cpp		\
		srcs/commands/Join.cpp			\
		srcs/commands/Privmsg.cpp		\
		srcs/commands/Nick.cpp			\
		srcs/commands/User.cpp			\
		srcs/commands/Part.cpp			\
		srcs/reply.cpp					\
		srcs/Channel.cpp				\

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g3 -MMD -fstandalone-debug -std=c++98

RM = rm -rf

OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

RED		:= \033[31m
YELLOW	:= \033[33m
GREEN	:= \033[32m
BLUE	:= \033[34m
RESET	:= \033[0m

all : $(NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -o $@ -c $< || (echo "$(BLUE)$(NAME): $(RED) $< Compilation failure$(RESET)" && return 1)

$(NAME): $(OBJS)
	@echo "$(BLUE)$(NAME): Compiling $(OBJS) $(NAME)$(RESET)"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(BLUE)$(NAME): $(GREEN)$(NAME) Compiled!$(RESET)"

v: re
	valgrind --show-leak-kinds=all -s --leak-check=full --track-origins=yes ./irc 9997 bleu


clean:
	@echo "$(BLUE)$(NAME): Cleaning object files$(RESET)"
	@$(RM) $(OBJS) $(DEPS) && clear

fclean: clean
	@echo "$(BLUE)$(NAME): Cleaning $(NAME)$(RESET)"
	@$(RM) $(NAME) && clear

re: fclean all

.PHONY: all clean fclean re v