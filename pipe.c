/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:16:02 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/16 16:20:06 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe(t_general *list, t_now *get, t_env **env)
{
	t_general	*tmp;
	int			**fd;
	pid_t		*pid;
	int			count;
	int			i;

	i = 0;
	count = 0;
	tmp = list;
	while (tmp && tmp->blocs)
	{
		count++;
		tmp = tmp->next;
	}
	fd = malloc(sizeof(int *) * (count - 1));
	pid = malloc(sizeof(pid_t) * count);
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
	tmp = list;
	i = 0;
	while (i < count)
	{
		pid[i] = fork();
		if (pid[i] == 0)
		{
			if (i == 0)
			{
				close(fd[i][0]);
				dup2(fd[i][1], 1);
				close(fd[i][1]);
				for (int j = 1; j < count - 1; j++)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
			}
			else if (i == count - 1)
			{
				close(fd[i - 1][1]);
				dup2(fd[i - 1][0], 0);
				close(fd[i - 1][0]);
				for (int j = 0; j < count - 2; j++)
                {
                    if (j != i-1)
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                }
			}
			else
			{
				close(fd[i - 1][1]);
				close(fd[i][0]);
				dup2(fd[i - 1][0], 0);
				dup2(fd[i][1], 1);
				close(fd[i - 1][0]);
				close(fd[i][1]);
				for (int j = 0; j < count - 1; j++)
                {
                    if (j != i-1 && j != i)
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                }
			}
			handle_redirections(tmp);
			if (is_built_in(tmp->acces_args->args[0]->str) == 1)
			{
				check_cmd_built_in(tmp, env);
				exit(tmp->dqm);
			}
			else
				execute_command(tmp, get);
		}
		i++;
		if (tmp)
			tmp = tmp->next;
	}
	for (int i = 0; i < count - 1; i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
	i = 0;
	while (i < count)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	for (i = 0; i < count - 1; i++)
        free(fd[i]);
    free(fd);
    free(pid);
}
