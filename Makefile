SRC = main.c \
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
	  unset.c \
	  signals.c \
	  parser.c	\
	  free_utils.c \
	  free_pipe.c \
	  pipe_first.c \
	  pipe_second.c \
	  execute_first.c \
	  execute_second.c \
	  append.c \
	  redirection_handle_first.c \
	  redirection_handle_second.c \
	  heredoc.c \
	  heredoc_second.c \
	  heredoc_third.c \
	  heredoc_fourth.c \
	  fill_nowstruct.c \
	  execute_handle_first.c \
	  execute_handle_second.c \
	  echo_builtin.c \
	  create_env.c \
	  environment_third.c \
	  cd_cmd.c \
	  export_ctrl.c \
	  cmd_builtin_scnd.c \
	  helper_function.c \
	  helper_function_second.c \
	  wq_parser.c \
	  sq_parser.c \
	  dq_parser.c \
	  parser_scnd.c \
	  expand_dollar.c \
	  expand_dollar_utils.c \
	  expand_dollar_second.c

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

leaks:
	@echo "Running walgrind..."
	@valgrind --leak-check=full			\
			--show-leak-kinds=all 		\
			--track-origins=yes			\
			--track-fds=yes				\
			--verbose 					\
			--suppressions=valgrind.supp	\
			./$(MINISHELL)


.PHONY: all clean fclean re
