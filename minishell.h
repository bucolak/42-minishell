/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:05:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/02 15:49:18 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"

// temel giriş/çıkış
#include <stdio.h>       // printf, perror
#include <stdlib.h>      // malloc, free, exit, getenv
#include <unistd.h>      // write, access, read, close, fork, getcwd, chdir, execve, isatty, ttyname, ttyslot

// dosya işlemleri
#include <fcntl.h>       // open, O_RDONLY, O_WRONLY, vs.
#include <sys/stat.h>    // stat, lstat, fstat
#include <dirent.h>      // opendir, readdir, closedir

// sinyal işlemleri
#include <signal.h>      // signal, sigaction, sigemptyset, sigaddset, kill

// işlem kontrolü
#include <sys/wait.h>    // wait, waitpid, wait3, wait4

// hata işlemleri
#include <string.h>      // strerror

// terminal özellikleri
#include <termios.h>     // tcsetattr, tcgetattr
#include <termcap.h>     // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <sys/ioctl.h>   // ioctl

// readline
#include <readline/readline.h>
#include <readline/history.h>


typedef struct s_env
{
    char *data;
    char *key;
    int is_exported;
    struct s_env *next;
}   t_env;

typedef struct s_arg
{
    char *str;
    int flag;
}   t_arg;

typedef struct s_pipeafter
{
    t_arg **args;
}   t_pipeafter;

typedef struct s_general
{
    int $qm;
    char *blocs;
    t_pipeafter *acces_args;
    struct s_general *next;
} t_general;


// apply_malloc.c
t_arg *create_arg(const char *str, int flag);
t_pipeafter *create_pipeafter(void);
t_general *create_general_node(void);
t_env *create_env_node(void);

// main.c
void dolar_control(t_general *a);
void pipe_parse(t_general **pipe_block, char *line);
void parse_input( t_general *a);

// execute.c
void check_cmd_sys_call(t_general *pipe_blocs);

// cmd_built_in.c
void check_cmd_built_in(t_general *pipe_blocs, t_env **node, char **envp);
void exit_cmd(t_general *list);
void unset_cmd(t_general *list, t_env **env);
void print_export_env(t_env **env);
t_env **export_cmd( t_env **env);
void cd_cmd(t_arg **args);
void pwd_cmd(char **ar);

// environment.c
void get_env(t_env **node, char **envp);
void print_env(t_general *list, t_env **node, char **envp, int i);
void	ft_envadd_back(t_env **lst, char *key, char *data);
char *get_key(char *str);
char *get_data(char *str);
void create_env(t_general *list, t_env **env);


//count_quote.c
int count_squote(char *str);
int count_dquote(char *str);

// message.c
int message(int i);

// etc.c
int	ft_lsttsize(t_env *lst);
int	ft_strcmp(const char *s1, const char *s2);
t_env	*ft_lsttlast(t_env *lst);
int is_numeric(char *str);
int is_repeated(t_env **node, char *ky, char *dt);

#endif