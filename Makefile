NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = main.cpp Sources/Server.cpp Sources/Client.cpp Sources/Utils.cpp Sources/Message.cpp Sources/Channel.cpp

OBJS = ${SRCS:.cpp=.o}

%.o: %.cpp
	${CC} ${FLAG} -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) compiled"

all:	$(NAME)

clean:
	rm -rf $(OBJS)
	@echo "$(NAME) cleaned"

fclean:	clean
	rm -rf $(NAME)
	@echo "$(NAME) fcleaned"

re:	fclean all clean
