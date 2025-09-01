NAME = ircserv

SOURCES = srcs/main.cpp \
		srcs/Parsing.cpp	\
		srcs/Server.cpp	\
		srcs/Client.cpp	\
		srcs/Channel.cpp	\
		srcs/Signal.cpp	\
		srcs/commands/Privmsg.cpp \
		srcs/commands/Pass.cpp	\
		srcs/commands/User.cpp	\
		srcs/commands/Nick.cpp	\
		srcs/commands/Join.cpp	\
		srcs/commands/Kick.cpp	\
		srcs/commands/Invite.cpp	\
		srcs/commands/Topic.cpp	\
		srcs/commands/Mode.cpp	\
		srcs/commands/Part.cpp	\
		srcs/commands/Quit.cpp \
		srcs/commands/ACommand.cpp

SRCS = $(addprefix $(OBJ_DIR)/, $(SOURCES))

OBJ_DIR = objs

OBJS = $(SRCS:.cpp=.o)

DEPS = $(SRCS:.cpp=.d)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -g3
CXXFLAGS	+=	-I $(HEAD_D)

HEAD_D	=		headers/

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
		@mkdir -p $(@D)
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME) $(OBJ_DIR)

re: fclean
	make all

-include $(DEPS)

.PHONY: all clean fclean re
