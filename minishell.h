/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:05:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/04/14 16:29:15 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include "ft_printf/ft_printf.h"

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

#endif