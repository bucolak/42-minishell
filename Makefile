SRC = main.c

CC = cc 

PRINTF= ft_printf/libftprintf.a

LIN

MINISHELL = minishell 

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all: $(MINISHELL)

$(MINISHELL): $(OBJ)
	make -C ./ft_printf
	$(CC) $(CFLAGS) $(OBJ) $(PRINTF) -o $(MINISHELL)

clean: 
	make clean -C ./ft_printf
	rm -f $(OBJ)

fclean: clean
	make fclean -C ./ft_printf
	rm -f $(MINISHELL)

re: fclean all

.PHONY: all clean fclean re