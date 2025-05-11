/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:05:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/11 20:55:32 by bucolak          ###   ########.fr       */
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
# include <sys/ioctl.h> // ioctl
# include <termcap.h>   // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <termios.h>   // tcsetattr, tcgetattr

// readline
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_env
{
	char				*data;
	char				*key;
	struct s_env		*next;
}						t_env;

typedef struct s_arg
{
	char				*str;
	int					flag;
}						t_arg;

typedef struct s_pipeafter
{
	t_arg				**args;
}						t_pipeafter;

typedef struct s_general
{
	int					dqm;
	char				*blocs;
	t_pipeafter			*acces_args;
	struct s_general	*next;
}						t_general;

typedef struct s_now
{
	char				**envp;
}						t_now;

typedef struct s_heredoc
{
	char *content;
	struct s_heredoc *next;
}	t_heredoc;

// apply_malloc.c
t_arg					*create_arg(const char *str, int flag);
t_pipeafter				*create_pipeafter(void);
t_general				*create_general_node(void);
t_env					*create_env_node(void);

// main.c
void					pipe_parse(t_general **pipe_block, char *line);
void					parse_input(t_general *a);

// execute.c
void					check_cmd_sys_call(t_general *pipe_blocs, t_env **env);
int is_built_in(char *str);

// cmd_built_in.c
void					check_cmd_built_in(t_general *pipe_blocs, t_env **node);
void					built_in_helper_func(t_general *pipe_blocs,
							t_env **node, int *i);
void					cd_cmd(t_arg **args);
void					pwd_cmd(char **ar);

// environment_first.c
void					get_env_helper_func(int *i, int *j, t_env *tmp,
							char **envp);
void					get_env(t_env **node, char **envp);
void					print_env(t_general *list, t_env **node, int i);
void					ft_envadd_back(t_env **lst, char *key, char *data);
void					create_env(t_general *list, t_env **env);

// environment_second.c
char					*get_key(char *str);
char					*get_data(char *str);
void					print_export_env(t_env **env);
void					export_cmd_helper_func(t_env **env, t_env **new_env,
							t_env *swap, int *j);
t_env					**export_cmd(t_env **env);

//count_quote.c
int						count_squote(char *str);
int						count_dquote(char *str);

// message.c
int						message(int i);

// etc.c
int						ft_lsttsize(t_env *lst);
int						ft_strcmp(const char *s1, const char *s2);
t_env					*ft_lsttlast(t_env *lst);
int						is_numeric(char *str);
int						is_repeated(t_env **node, char *ky, char *dt);

// redirection.c
void					renew_block(char ***args);
void					renew_block2(t_general *list);

// echo_first.c
void					echo_flag_4_second(char *str, char *env, t_general *tmp,
							int *j);
void					echo_flag_4(char *str, char *env, t_general *tmp);
void					echo_cmd(t_general *tmp, char *str, char *env, int i);
void					initalized_echo(t_general *list);

// echo_second.c
void					echo_flag_0_and_2_second(char *str, char *env,
							t_general *tmp, int *j);
void					echo_flag_0_and_2(char *env, char *str, t_general *tmp,
							int i);
void					echo_flag_1(char *env, t_general *tmp, int i);

// exit.c
void					exit_cont(t_general *list, int a);
void					exit_cmd(t_general *list);

// unset.c
void					unset_cmd_helper_func(t_env *node, t_env *pre_node,
							char *s);
void					unset_cmd(t_general *list, t_env **env);

#endif