/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/18 18:34:09 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_heredoc(t_general *list)
{
	int		i;
	int		j;
	t_arg	**new_arg;
	t_arg **old_arg;
	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	i = 0;
	old_arg = list->acces_args->args;
	while (list->acces_args->args[i])
	{
		i++;
	}
	new_arg = malloc(sizeof(t_arg *) * (i + 1));
	if(!new_arg)
		return ;
	i = 0;
	j = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
		{
			i += 2;
			continue ;
		}
		new_arg[j++] = list->acces_args->args[i++];
	}
	new_arg[j] = NULL;
	list->acces_args->args = new_arg;
	i = 0;
	while(old_arg[i])
	{
		if(ft_strcmp(old_arg[i]->str,"<<") == 0)
		{
			free(old_arg[i]->str);
			free(old_arg[i]);
			i++;
			if(old_arg[i])
			{
				free(old_arg[i]->str);
				free(old_arg[i]);
				i++;
			}
			continue;
		}
		i++;
	}
	free(old_arg);
}

void	fill_limiter(t_general *list)
{
	int	i;
	int	j;
	int	c;

	i = 0;
	j = 0;
	c = 0;
	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	while(list->acces_args->args[i])
	{
		if(ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			c++;
		i++;
	}
	list->limiter = malloc(sizeof(char *)*(c +1));
	i = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
		{
			if (list->acces_args->args[i + 1])
			{
				list->limiter[j] = ft_strdup(list->acces_args->args[i
						+ 1]->str);
				j++;
			}
		}
		i++;
	}
	list->limiter[j] = NULL;
}

void signal_handler_heredoc(int signo)
{
	if(signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
        rl_replace_line("", 0);
        //rl_redisplay();
		 exit(130);
	}
}

void	handle_heredoc(t_general *list)
{
	int		i;
	int		fd[2];
	int j;
	int s = 0;
	char	*line;
	t_general *tmp;
	tmp = list;
	while (tmp)
	{
		fill_limiter(tmp);
		s++;
		i = 0;
		j = 0;
		while (tmp->acces_args->args[i])
		{
			if (ft_strcmp(tmp->acces_args->args[i]->str, "<<") == 0)
			{
				tmp->flag_heredoc = 1;
				if (!tmp->acces_args->args[i + 1])
				{
					ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
							2);
					return ;
				}
				pipe(fd);
				while (1)
				{
					signal(SIGINT,signal_handler_heredoc);
					signal(SIGQUIT, SIG_DFL);
					line = readline("heredoc > ");
					if (!line || ft_strcmp(line, tmp->limiter[j]) == 0)
					{
						free(line);
						break ;
					}
					ft_putstr_fd(line, fd[1]);
					ft_putstr_fd("\n", fd[1]);
					free(line);
				}
				close(fd[1]);
				j++;
				if (!tmp->limiter[j])
				{
					tmp->heredoc_fd = dup(fd[0]);
				}
				close(fd[0]);
				i += 2;
				continue ;
			}
			i++;
		}
		remove_heredoc(tmp);
		tmp = tmp->next;
	}
}
