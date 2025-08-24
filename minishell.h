/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:05:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 19:09:21 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

// temel giriş/çıkış
# include <stdio.h>  // printf, perror
# include <stdlib.h> // malloc, free, exit, getenv
# include <unistd.h> // write, access, read, close, fork, getcwd, chdir, execve,
					// isatty, ttyname, ttyslot

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
	int					f;
	struct s_env		*next;
	int					has_equal;
}						t_env;

typedef struct s_arg
{
	char				*str;
	int					flag;
	int					env_flag;
	int					s;
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
	int					a;
	struct s_general	*next;
	int					heredoc_fd;
	int					flag_heredoc;
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
	t_general			*pipe_blocks;
	t_env				*node;
	t_pipe				*pipe;
	t_now				*get;
	char				**new;
}						t_full;

typedef struct s_hdoc_state
{
	int					i;
	int					j;
	int					status;
}						t_hdoc_state;

typedef struct s_state
{
	t_general			*pipe_blocs;
	t_env				*env;
	t_now				*get;
	t_full				full;
	t_pipe				*pipe;
	int					last_dqm;
}						t_state;

// extern int				signal_ec;

// export_ctrl.c
int						ctrl_is_valid_identifier_builtin_helper(t_general *tmp,
							int i);
void					export_cntrl_builtin_helper(t_general *tmp, int i,
							int *c, t_full *full);
void					export_cntrl_builtin_helper_scnd(t_full *full,
							t_general *tmp);
void					put_flag_builtin_helper(t_full *full);

// main_scnd.c
void					init_var(t_full *full, t_pipe **pipe, t_now **get,
							t_general **pipe_blocs);
void					init_env(t_env **env, char **envp, t_full *full,
							int *first_run);
void					ctrld_free_handler(t_now **get, t_env **env,
							t_pipe **pipe);
void					ctrld_free_exit(t_general **pipe_blocs);
int						main_line_ctrl_scnd(char *line, t_general **pipe_blocs);

// main_third.c
int	apply_parser(char *line, t_general *pipe_blocs, t_env *env,
		t_full *full);
void					fill_get(t_now **get, t_env *env, t_full *full);
void					apply_pipe(t_general *pipe_blocs, t_pipe **pipe,
							t_full *full, t_env **env);
void					cleanup_loop_end(t_now **get, t_general **pipe_blocs,
							char **line, int *last_dqm);

// parser.c
char					*clean_double_quotes(char *str);
int	parse_input(t_general *a);

// parser_scnd.c
int						is_redireciton2(char *str);
int						count_args(const char *str);

// dq_parser.c
int					handle_double_quotes_parser(t_general *a, int *i,
							int *j, int *k);

// sq_parser.c
int					handle_single_quotes_parser(t_general *a, int *i,
							int *j, int *k);

// wq_parser.c
void					handle_without_quotes_parser(t_general *a, int *i,
							int *j, int *k);

// execute_handle_second.c
void					expand_dollar_qmark_execute(t_general *list);
//
// environment_first.c
int						key_cont(char *key);
//
// environment_third.c
char					*get_key(char *str);
char					*get_data(char *str);
//
void					print_message_and_exit(int type, t_general *pipe_blocs,
							t_full *full, int j);
void					print_message_and_exit2(t_general *pipe_blocs,
							t_full *full, int j);
char					**make_argv(t_pipeafter *acces_args, t_env *env);
void					path_control_execute(char **args, t_general *pipe_blocs,
							char *cmd, t_full *full);
void					control_redirecitons_and_redirect_to_func(t_general *pipe_blocs,
							int *is_redirect, t_full *full, int i);
void					go_to_execve_execute(char **paths, int *command_found,
							t_full *full, t_general *pipe_blocs);
void					if_command_not_found_execute(int command_found,
							t_general *pipe_blocs, t_full *full, char ***paths);
void					heredoc_fd_dup(t_general *list);
void					direct_and_close_fd(int count, int **fd, int i,
							int type);
void					clean_and_exit(t_full *full, int ec);
void					close_fd(int count, int **fd, int type, int i);
void					end_block(int count, int i, int **fd);
void					wait_child_pipe(t_pipe *pipe, t_general *list);
void					free_for_heredoc(t_full *full);
void					close_all_open_fds(void);
void					signal_handler_heredoc(int signo);
void					close_heredoc_fd(t_general *list);
void					cleanup(t_full *full);
void					control_redireciton(t_general *list, t_env *env);
int						is_flag_6(t_general *list, t_env *env);
int						has_redireciton(t_general *pipe_blocks);
char					*ft_strstr(const char *haystack, const char *needle);
void					remove_env_var(t_env **env, char *var_name);
void					signal_handler(void);
void					print_pipes(t_general *pipe_block);
char					*get_getenv(t_env *env, char *key);
void					free_split(char **str);
// apply_malloc.c
// t_arg *create_arg(const char *str, int flag);
t_arg					*create_arg(char *str, int flag, int type);
t_pipeafter				*create_pipeafter(void);
t_general				*create_general_node(int dqm);
t_env					*create_env_node(void);

// main.c
void					pipe_parse(t_general **pipe_block, char *line);
int						has_heredoc(t_general *list);
void					expand_dollar(t_general *list, t_env *env);
void					connect_count_malloc(t_general *list);
void					remove_null(t_general *list);
void					init_pipe(t_pipe *pipe, t_general *list);
void					create_pipe(int count, int **fd);
char					*get_getenv(t_env *env, char *key);
int						count_m(t_general *tmp, int i, t_env *env);

// execute.c
void					check_cmd_sys_call(t_general *pipe_blocs, t_env *env,
							t_pipe *pipe, t_full *full);
int						is_built_in(char *str);
void					fill_env(t_env *env, t_now *get);
void					handle_pipe(t_general *list, t_env *env, t_pipe *pipe,
							t_full *full);
void					handle_redirections(t_general *pipe_blocs,
							t_full *full);
void					execute_command(t_general *pipe_blocs, t_now *get,
							t_env *envv, t_full *full);
// cmd_built_in.c
void					check_cmd_built_in(t_general *pipe_blocs, t_env **node,
							t_pipe *pipe, t_now *get);
void					cd_cmd(t_arg **args, t_env *env,
							t_general *pipe_blocks);
void					pwd_cmd(char **ar, t_general *list, t_env *env);

// environment_first.c
void					get_env(t_env *node, char **envp);
void					print_env(t_general *list, t_env *node, int i);
void					ft_envadd_back(t_env **lst, char *key, char *data,
							t_full *full);
void					create_env(t_general *list, t_env **env, t_full *full);

// environment_second.c
char					*get_key(char *str);
char					*get_data(char *str);
void					print_export_env(t_env *env, t_general *list);
t_env					**export_cmd(t_env *env);

// count_quote.c
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
int						is_repeated(t_env *node, char *ky, char *dt);

// redirection_first.c
int						is_redirection(char *str);
void					renew_block2(t_general *list);
void					renew_else_block(t_arg ***new, t_general *tmp, int *i,
							int *j);
void					renew_block2(t_general *list);
void					handle_input(t_general *list, int i, t_full *full);
void					handle_output(t_general *list, int i, t_full *full);
void					handle_append(t_general *list, int i, t_full *full);

// redirection_second.c
void					handle_heredoc(t_general *list, t_full *full);

// echo_first.c
void					echo_flag_4_second(char *str, t_general *tmp, int *j);
void					echo_cmd(t_general *tmp, int i);
void					initalized_echo(t_general *list);
int						cont_n(char *str);

// echo_second.c
void					echo_flag_0_and_2(char *str, t_general *tmp, int i);
void					echo_flag_1(t_general *tmp, int i);

// exit.c

void					exit_cmd(t_full *full, t_general *list);

// unset.c
void					unset_cmd_helper_func(t_env *node, t_env *pre_node,
							char *s);
void					unset_cmd(t_general *list, t_env **env);

void					handle_signal(int signo);
void					remove_heredoc(t_general *list);

// free.c
void					free_env(t_env *env);
void					free_pipe_blocks(t_general *blocks);
void					free_pipe(t_pipe *pipe);
void					free_envp(t_now *get);
void					free_acces_args(t_pipeafter *acc);
void					free_limiter(char **limiter);

// heredoc_third.c
int						process_heredocs(t_general *tmp, t_full *full);

// heredoc_fourth.c
int						handle_heredoc_process(t_general *tmp, t_full *full,
							int *j, int *status);
void					handle_heredoc(t_general *list, t_full *full);

// heredoc.c
void					remove_heredoc(t_general *list);
int						go_to_handle_heredoc(t_general *list, t_full *full);

// heredoc_second.c
void					fill_limiter(t_general *list);
void					free_heredoc(t_full *full);
void					signal_handler_heredoc(int signo);
int						skip_expansion(t_general *node, int i);
int						process_single_arg(t_general *node, int i, t_env *env);
char					*expand_variables_in_string(t_arg *arg, t_env *env);
int						expanded_length(t_arg *arg, t_env *env);
char					*build_expanded_string(t_arg *arg, t_env *env,
							char *new_str);
int						is_expandable_variable(t_arg *arg, int pos);
int						expand_variable_at_position(t_arg *arg, t_env *env,
							char *dest, int *pos);
int						remove_empty_arg(t_general *node, int i);
void					expand_args_in_node(t_general *node, t_env *env);

#endif