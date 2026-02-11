# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brice <brice@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/22 13:15:03 by brice             #+#    #+#              #
#    Updated: 2026/01/29 11:09:29 by brice            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---------- Colors ----------
RESET  = \033[0m
BOLD   = \033[1m
WHITE  = \033[37m
GREEN  = \033[32m

# ---------- Project ----------
NAME    = ft_ping
CC      = cc
CFLAGS  = -Wall -Wextra -Werror

INCLUDES = -Iinclude -Ilib

LIBFT_DIR = lib
LIBFT     = $(LIBFT_DIR)/libft.a

SRC = main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

# Build ft_ping + your banner
$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "\n$(WHITE)$(BOLD)"
	@printf "%s\n" "	                        _,-}}-._ " \
	"	                       /\\   }  /\\ " \
	"	                     _|(O\\_   _/O)                 ___|_ " \
	"	                    _|/  (__''__)                 |#####| " \
	"	                  _|\\/    WVVVVW                  |#####| " \
	"	                 \\ _\\     \\MMMM/_             .-----.###| " \
	"	               _|\\_\\     _ '---; \\_           |#####|###| " \
	"	          /\\   \\ _\\/      \\_   /   \\          |#####|###| " \
	"	         / (    _\\/     \\   \\  |'VVV     P    |#####|###| " \
	"	        (  '-,._\\_.(      'VVV /      (_/|\\_) |#####|###| " \
	"	         \\         /   _) /   _)     (_/_ _\\_)|#####|###| " \
	"	          '....--''\\__vvv)\\__vvv)_____|_| |_|_|#####|###| "
	@echo "$(RESET)"
	@echo "$(GREEN)$(BOLD)Build complete! ✨$(RESET)"

# Build libft automatically
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
