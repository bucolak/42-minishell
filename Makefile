SRC = main.c \
	  execute.c \
	  cmd_built_in.c \
	  environment_first.c \
	  environment_second.c \
	  count_quote.c \
	  message.c \
	  etc.c \
	  apply_malloc.c \
	  redirection_first.c \
	  redirection_second.c \
	  echo_first.c \
	  echo_second.c \
	  exit.c \
	  unset.c

CC = gcc 

LIBFT = libft/libft.a

MINISHELL = minishell 

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

LDFLAGS = -lreadline

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(MINISHELL)
	@echo "Compiled!"
	
$(MINISHELL): $(OBJ)
	@make -C ./libft --silent
	@make bonus -C ./libft --silent
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(MINISHELL)

clean: 
	@make clean -C ./libft --silent
	@rm -f $(OBJ)

fclean: clean
	@make fclean -C ./libft --silent
	@rm -f $(MINISHELL)

re: fclean all
	@echo "Compiled!"

.PHONY: all clean fclean re
