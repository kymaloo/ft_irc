NAME = client

SRCS =    main.cpp    \

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g3 -MMD -std=c++98

RM = rm -f

OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all : $(NAME)
    
$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean : 
	$(RM) $(OBJS) $(DEPS)

fclean : clean
	$(RM) $(NAME)

re : fclean all