SRC = main.c execute.c cmd_built_in.c environment_first.c environment_second.c count_quote.c message.c etc.c apply_malloc.c redirection.c echo_first.c echo_second.c exit.c

CC = gcc 

LIBFT = libft/libft.a

MINISHELL = minishell 

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

LDFLAGS = -lreadline

all: $(MINISHELL)

$(MINISHELL): $(OBJ)
	make -C ./libft
	make bonus -C ./libft
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(MINISHELL)

clean: 
	make clean -C ./libft
	rm -f $(OBJ)

fclean: clean
	make fclean -C ./libft
	rm -f $(MINISHELL)

re: fclean all

.PHONY: all clean fclean re
