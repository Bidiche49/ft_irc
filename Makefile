# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: audrye <audrye@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 14:13:00 by audrye            #+#    #+#              #
#    Updated: 2024/09/05 16:22:25 by audrye           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++98

CREADLINE = -lreadline

RESET   = \033[0m
GREEN   = \033[32m
RED     = \033[31m
YELLOW  = \033[33m
CYAN    = \033[36m

SRCS    =	main.cpp			\
			./srcs/client.cpp	\
			./srcs/command.cpp	\
			./srcs/utils.cpp	\
			./srcs/channel.cpp	\
			./srcs/errors.cpp	\
			./srcs/server.cpp

INCDIR  = ./incs

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling in progress...$(RESET)"
	@{ $(CC) $(CFLAGS) -I$(INCDIR) $(OBJS) $(CREADLINE) -o $(NAME) || \
		(echo "$(RED)Compilation failed.$(RESET)"; exit 1); }
	@echo "$(GREEN)Compilation completed.$(RESET)"

%.o: %.cpp
	@$(CC) $(CFLAGS) -I$(INCDIR) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	@echo "$(YELLOW)Removing object files and dependency files...$(RESET)"
	@rm -f $(OBJS) $(DEPS)
	@echo "$(GREEN)Object files and dependency files removed.$(RESET)"

fclean: clean
	@echo "$(YELLOW)Removing the executable...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)Executable removed.$(RESET)"

re: fclean all
