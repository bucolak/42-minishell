/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function_second.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:50:13 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 20:51:45 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_flag_6_arg(char *arg_str, t_env *env)
{
	char	*str;
	t_env	*tenv;
	int		j;

	str = NULL;
	j = 0;
	while (arg_str[j])
	{
		if (arg_str[j] == '=' && arg_str[j + 1])
		{
			str = arg_str + ++j;
			break ;
		}
		j++;
	}
	tenv = env;
	while (tenv)
	{
		if (ft_strncmp(str, tenv->key, ft_strlen(tenv->key)) == 0)
		{
			tenv->f = 3;
			break ;
		}
		tenv = tenv->next;
	}
}

void	control_redireciton(t_general *list, t_env *env)
{
	t_general	*tmp;
	int			i;

	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (tmp->acces_args->args[i]->flag == 6)
				process_flag_6_arg(tmp->acces_args->args[i]->str, env);
			i++;
		}
		tmp = tmp->next;
	}
}

void	create_pipe(int count, int **fd)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	init_pipe(t_pipe *pipe, t_general *list)
{
	pipe->count = 0;
	pipe->tmp = list;
	while (pipe->tmp)
	{
		pipe->count++;
		pipe->tmp = pipe->tmp->next;
	}
	pipe->fd = malloc(sizeof(int *) * (pipe->count - 1));
	pipe->pid = malloc(sizeof(pid_t) * pipe->count);
}

void	signal_handler(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}
