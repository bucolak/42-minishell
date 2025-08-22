/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_third.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/22 22:00:01 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_arg	**build_heredoc_clean_args(t_arg **args)
{
	t_arg	**new_arg;
	int		i;
	int		j;
	int		count;

	count = 0;
	while (args[count])
		count++;
	new_arg = malloc(sizeof(t_arg *) * (count + 1));
	if (!new_arg)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i]->str, "<<") == 0)
		{
			i += 2;
			continue ;
		}
		new_arg[j++] = args[i++];
	}
	new_arg[j] = NULL;
	return (new_arg);
}
static void	free_heredoc_args(t_arg **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i]->str, "<<") == 0)
		{
			free(args[i]->str);
			free(args[i]);
			i++;
			if (args[i])
			{
				free(args[i]->str);
				free(args[i]);
				i++;
			}
			continue ;
		}
		i++;
	}
	free(args);
}
void	remove_heredoc(t_general *list)
{
	t_arg	**old_args;
	t_arg	**new_args;

	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	old_args = list->acces_args->args;
	new_args = build_heredoc_clean_args(old_args);
	if (!new_args)
		return ;
	list->acces_args->args = new_args;
	free_heredoc_args(old_args);
}
static int	count_heredoc(t_general *list)
{
	int	i;
	int	count;

	if (!list || !list->acces_args || !list->acces_args->args)
		return (0);
	i = 0;
	count = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			count++;
		i++;
	}
	return (count);
}
void	fill_limiter(t_general *list)
{
	int	i;
	int	j;
	int	heredoc_count;

	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	heredoc_count = count_heredoc(list);
	list->limiter = malloc(sizeof(char *) * (heredoc_count + 1));
	if (!list->limiter)
		return ;
	i = 0;
	j = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0
			&& list->acces_args->args[i + 1])
		{
			list->limiter[j] = ft_strdup(list->acces_args->args[i + 1]->str);
			j++;
		}
		i++;
	}
	list->limiter[j] = NULL;
}

void	free_heredoc(t_full *full)
{
	static t_full	*temp_full;

	if (full)
	{
		temp_full = full;
	}
	else
	{
		cleanup(temp_full);
		if (temp_full->pipe_blocks)
			free_pipe_blocks(temp_full->pipe_blocks);
		exit(130);
	}
}
void	signal_handler_heredoc(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		signal_ec = 1;
		free_heredoc(NULL);
		exit(130);
	}
}

static void	cleanup_heredoc_exit(t_full *full, int fd)
{
	close(fd);
	cleanup(full);
	free_pipe_blocks(full->pipe_blocks);
	exit(0);
}

static void	read_heredoc_input(t_full *full, int fd_write, char *limiter)
{
	char	*line;

	free_heredoc(full);
	close(fd_write - 1);
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("heredoc > ");
		if (!line)
			cleanup_heredoc_exit(full, fd_write);
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			cleanup_heredoc_exit(full, fd_write);
		}
		ft_putstr_fd(line, fd_write);
		ft_putstr_fd("\n", fd_write);
		free(line);
	}
}
static int	handle_heredoc_process(t_general *tmp, t_full *full, int *j,
		int *status)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		read_heredoc_input(full, fd[1], tmp->limiter[*j]);
	close(fd[1]);
	waitpid(pid, status, 0);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(*status) && WEXITSTATUS(*status) == 130)
	{
		tmp->a = 1;
		close(fd[0]);
		remove_heredoc(tmp);
		tmp->heredoc_fd = -1;
		tmp->dqm = 130;
		return (1);
	}
	tmp->heredoc_fd = fd[0];
	return (0);
}
static int	process_heredocs(t_general *tmp, t_full *full)
{
	int	i;
	int	j;
	int	status;

	i = 0;
	j = 0;
	status = 0;
	while (tmp->acces_args->args[i])
	{
		if (ft_strcmp(tmp->acces_args->args[i]->str, "<<") == 0)
		{
			tmp->flag_heredoc = 1;
			if (!tmp->acces_args->args[i + 1])
			{
				ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
					2);
				return (1); // Hata varsa 1 döndür
			}
			if (handle_heredoc_process(tmp, full, &j, &status))
				return (1);
			j++;
			i += 2;
		}
		else
			i++;
	}
	remove_heredoc(tmp);
	return (0);
}

void	handle_heredoc(t_general *list, t_full *full)
{
	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	fill_limiter(list);
	process_heredocs(list, full);
}
