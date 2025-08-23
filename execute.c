/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/08/23 13:20:52 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_fd(t_general *list)
{
	t_general	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->heredoc_fd != -1)
		{
			close(tmp->heredoc_fd);
			tmp->heredoc_fd = -1;
		}
		tmp = tmp->next;
	}
}

void	fill_env(t_env *env, t_now *get)
{
	int		j;
	t_env	*tmp;
	int		key_len;
	int		data_len;

	j = 0;
	tmp = env;
	while (tmp)
	{
		key_len = 0;
		data_len = 0;
		if (!tmp->key || !tmp->data)
		{
			tmp = tmp->next;
			continue ;
		}
		key_len = ft_strlen(tmp->key);
		data_len = ft_strlen(tmp->data);
		get->envp[j] = malloc(key_len + data_len + 2);
		ft_strlcpy(get->envp[j], tmp->key, ft_strlen(tmp->key) + 1);
		get->envp[j][ft_strlen(tmp->key)] = '=';
		ft_strlcpy(get->envp[j] + ft_strlen(tmp->key) + 1, tmp->data,
			ft_strlen(tmp->data) + 1);
		if (!get->envp[j])
			return ;
		tmp = tmp->next;
		j++;
	}
	get->envp[j] = NULL;
}

int	count_malloc(t_general *list, int j)
{
	int		c;
	int		i;
	char	*str;
	char	*itoa;

	i = 0;
	c = 0;
	if (list->acces_args->args[j])
	{
		str = list->acces_args->args[j]->str;
		while (str[i])
		{
			if (str[i] == '$' && str[i] && str[i] == '?')
			{
				itoa = ft_itoa(list->dqm);
				c += ft_strlen(itoa);
				free(itoa);
				i++;
			}
			else
				c++;
			i++;
		}
	}
	return (c);
}

void	expand_dolar_qmark(t_general *list)
{
	char	*str;
	int		i;
	int		j;
	int		k;
	char	*new;

	j = 0;
	while (list->acces_args->args[j])
	{
		i = 0;
		k = 0;
		new = malloc(sizeof(char *) * (count_malloc(list, j) + 1));
		str = list->acces_args->args[j]->str;
		while (str[i])
		{
			if (str[i] == '$' && str[i + 1] && str[i + 1] == '?')
			{
				ft_memcpy(new + k, ft_itoa(list->dqm),
					ft_strlen(ft_itoa(list->dqm)));
				i += 2;
				k += ft_strlen(ft_itoa(list->dqm));
			}
			else
				new[k++] = str[i++];
		}
		new[k] = '\0';
		free(list->acces_args->args[j]->str);
		list->acces_args->args[j]->str = ft_strdup(new);
		free(new);
		j++;
	}
}

void	signal_handler_child(int a)
{
	(void)a;
	// write(1,"\n",1);
	// signal(SIGINT, SIG_DFL);
	// signal(SIGQUIT, SIG_DFL);
}

void	check_cmd_sys_call(t_general *pipe_blocs, t_env *env, t_pipe *pipe,
		t_full *full)
{
	int			status;
	pid_t		pid;
	int			exit_code;
	t_general	*tmp;

	status = 0;
	tmp = pipe_blocs;
	while (tmp)
	{
		if (has_heredoc(tmp) == 1)
		{
			handle_heredoc(tmp, full);
		}
		if (tmp->a == 1)
			return ;
		tmp = tmp->next;
	}
	pid = fork();
	if (pipe_blocs->next)
	{
		handle_pipe(pipe_blocs, env, pipe, full);
		return ;
	}
	if (pid == 0)
	{
		if (pipe_blocs->a == 1)
		{
			// cleanup(full);
			// free_pipe_blocks(full->pipe_blocks);
			exit(130);
		}
		signal(SIGINT, SIG_DFL);
		if (has_redireciton(pipe_blocs) == 1 && is_flag_6(pipe_blocs, env) == 0)
		{
			handle_redirections(pipe_blocs, full);
		}
		if (pipe_blocs->heredoc_fd != -1)
		{
			dup2(pipe_blocs->heredoc_fd, 0);
			close(pipe_blocs->heredoc_fd);
			pipe_blocs->heredoc_fd = -1;
		}
		if (!pipe_blocs->next || (pipe_blocs->next
				&& has_redireciton(pipe_blocs) == 1))
		{
			if (is_built_in(pipe_blocs->acces_args->args[0]->str) == 1)
			{
				check_cmd_built_in(pipe_blocs, &env, pipe, full->get);
				exit_code = pipe_blocs->dqm;
				cleanup(full);
				free_pipe_blocks(full->pipe_blocks);
				exit(exit_code);
			}
			else
			{
				expand_dolar_qmark(pipe_blocs);
				execute_command(pipe_blocs, full->get, env, full);
				cleanup(full);
				exit_code = pipe_blocs->dqm;
				free_pipe_blocks(full->pipe_blocks);
				exit(exit_code);
			}
		}
	}
	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) || WIFSIGNALED(status))
	{
		// Eğer sinyal ile sonlandıysa ve sinyal SIGINT (Ctrl+C) ise
		if (WIFSIGNALED(status))
		{
			write(1, "\n", 1); // Sadece bu durumda newline yazdır
			pipe_blocs->dqm = 130;
		}
	}
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		pipe_blocs->dqm = WEXITSTATUS(status);
}
