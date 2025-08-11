/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:05:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/11 17:08:14 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

// temel giriş/çıkış
# include <stdio.h>  // printf, perror
# include <stdlib.h> // malloc, free, exit, getenv
# include <unistd.h> // write, access, read, close, fork, getcwd, chdir, execve,
					//isatty, ttyname, ttyslot

// dosya işlemleri
# include <dirent.h>   // opendir, readdir, closedir
# include <fcntl.h>    // open, O_RDONLY, O_WRONLY, vs.
# include <sys/stat.h> // stat, lstat, fstat

// sinyal işlemleri
# include <signal.h> // signal, sigaction, sigemptyset, sigaddset, kill

// işlem kontrolü
# include <sys/wait.h> // wait, waitpid, wait3, wait4

// hata işlemleri
# include <string.h> // strerror

// terminal özellikleri
//# include <sys/ioctl.h> // ioctl
// # include <termcap.h>   // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
// # include <termios.h>   // tcsetattr, tcgetattr

// readline
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_env
{
	char				*data;
	char				*key;
	int f;
	struct s_env		*next;
	int has_equal;
}						t_env;

typedef struct s_arg
{
	char				*str;
	int					flag;
	int env_flag;
	int s;
}						t_arg;

typedef struct s_pipeafter
{
	t_arg				**args;
}						t_pipeafter;

typedef struct s_general
{
	int					dqm;
	char				*blocs;
	char				**limiter;
	t_pipeafter			*acces_args;
	struct s_general	*next;
	int					heredoc_fd;
}						t_general;

typedef struct s_now
{
	char				**envp;
}						t_now;

typedef struct s_pipe
{
	int					**fd;
	pid_t				*pid;
	int					count;
	t_general			*tmp;
}						t_pipe;

typedef struct s_full
{
	t_general *pipe_blocks;
	t_env *node;
	t_pipe *pipe;
	t_now *get;
}	t_full;

void control_redireciton(t_general *list, t_env *env);
int is_flag_6(t_general *list, t_env *env);
int	has_redireciton(t_general *pipe_blocks);
char	*ft_strstr(const char *haystack, const char *needle);
void remove_env_var(t_env **env, char *var_name);
void	signal_handler(void);
void	print_pipes(t_general *pipe_block);
char *get_getenv(t_env *env, char *key);
void					free_split(char **str);
// apply_malloc.c
//t_arg *create_arg(const char *str, int flag);
t_arg *create_arg(char *str, int flag, int type);
t_pipeafter				*create_pipeafter(void);
t_general				*create_general_node(int dqm);
t_env					*create_env_node(void);

// main.c
void					pipe_parse(t_general **pipe_block, char *line);
void	parse_input(t_general *a);
int has_heredoc(t_general *list);

// execute.c
void	check_cmd_sys_call(t_general *pipe_blocs, t_env **env, t_now *get, t_pipe *pipe,t_full *full);
int						is_built_in(char *str);
void					fill_env(t_env **env, t_now *get);
void	handle_pipe(t_general *list, t_now *get, t_env **env, t_pipe *pipe, t_full *full);
void					handle_redirections(t_general *pipe_blocs);
void	execute_command(t_general *pipe_blocs, t_now *get, t_pipe *pipe, t_env *envv, t_full *full);
// cmd_built_in.c
void	check_cmd_built_in(t_general *pipe_blocs, t_env **node, t_pipe *pipe, t_now *get);
void	cd_cmd(t_arg **args, t_env *env, t_general *pipe_blocks);
void	pwd_cmd(char **ar, t_general *list, t_env *env);

// environment_first.c
void					get_env_helper_func(int *i, int *j, t_env *tmp,
							char **envp);
void					get_env(t_env **node, char **envp);
void					print_env(t_general *list, t_env **node, int i);
void					ft_envadd_back(t_env **lst, char *key, char *data,
							t_general *list);
void					create_env(t_general *list, t_env **env);

// environment_second.c
char	*get_key(char *str);
char					*get_data(char *str);
void					print_export_env(t_env **env, t_general *list);
void					export_cmd_helper_func(t_env **env, t_env **new_env,
							t_env *swap, int *j);
t_env					**export_cmd(t_env **env);

//count_quote.c
int						count_squote(char *str);
int						count_dquote(char *str);

// message.c
int						message(int i);
void					error_msg(int i, char *str, int type, t_general *list);

// etc.c
int						ft_lsttsize(t_env *lst);
int						ft_strcmp(const char *s1, const char *s2);
t_env					*ft_lsttlast(t_env *lst);
int						is_numeric(char *str);
int						is_repeated(t_env **node, char *ky, char *dt);

// redirection_first.c
int	is_redireciton(char *str);
void					renew_block2(t_general *list);
void					renew_else_block(t_arg ***new, t_general *tmp, int *i,
							int *j);
void					renew_block2(t_general *list);
void	handle_input(t_general *list, int i);
void	handle_output(t_general *list, int i);
void	handle_append(t_general *list, int i);

// redirection_second.c
void					handle_heredoc(t_general *list);

// echo_first.c
void	echo_flag_4_second(char *str, t_general *tmp, int *j);
void	echo_cmd(t_general *tmp, char *str, int i);
void	initalized_echo(t_general *list);

// echo_second.c
void					echo_flag_0_and_2_second(char *str,
							t_general *tmp, int *j);
void					echo_flag_0_and_2(char *str, t_general *tmp,
							int i);
void					echo_flag_1(t_general *tmp, int i);

// exit.c

void	exit_cmd(t_general *list, t_env *env, t_pipe *pipe, t_now *get);

// unset.c
void					unset_cmd_helper_func(t_env *node, t_env *pre_node,
							char *s);
void					unset_cmd(t_general *list, t_env **env);

void					handle_signal(int signo);
void					remove_heredoc(t_general *list);

//free.c
void free_env(t_env *env);
void free_pipe_blocks(t_general *blocks);
void free_pipe(t_pipe *pipe);
void free_envp(t_now *get);

#endif