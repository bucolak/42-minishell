/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_first.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:51:00 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/05 14:57:26 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redireciton(char *str)
{
	int	i;

	char *redireciton[] = {"<",
							"<<",
							">",
							">>",
							NULL};
	i = 0;
	while (redireciton[i])
	{
		if (ft_strcmp(redireciton[i], str) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	renew_else_block(t_arg ***new, t_general *tmp, int *i, int *j)
{
	(*new)[*j] = malloc(sizeof(t_arg));
	(*new)[*j]->str = ft_strdup(tmp->acces_args->args[*i]->str);
	(*new)[*j]->flag = tmp->acces_args->args[*i]->flag;
	(*new)[*j]->s = tmp->acces_args->args[*i]->s;  
}

void	renew_block2(t_general *list)
{
	int			i;
	t_arg		**new;
	t_general	*tmp;
	int			j;

	i = 0;
	j = 0;
	tmp = list;
	while (tmp->acces_args->args[i])
		i++;
	//new = malloc(sizeof(t_arg *) * (i + 1));
	new = ft_calloc(sizeof(t_arg *), (i + 1));
	i = 0;
	while (tmp->acces_args->args[i])
	{
		if (is_redireciton(tmp->acces_args->args[i]->str) == 1)
			i += 2;
		else
		{
			renew_else_block(&new, tmp, &i, &j);
			j++;
			i++;
		}
	}
	new[j] = NULL;
	tmp->acces_args->args = new;
	//free(new);
}

void	handle_output(t_general *list, int i)
{
	int	fd;
	char *last_input;
	int exit_code;
	int last_fd;
	last_fd = -1;
	if (ft_strcmp(list->acces_args->args[i]->str, ">") == 0)
	{
		if (list->acces_args->args[i + 1])
		{
			i++;
			last_input = list->acces_args->args[i]->str;
			fd = open(last_input, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if(access(last_input, F_OK) != 0)
			{
				error_msg(2, last_input, 0, list);
				exit_code = list->dqm;
				free_pipe_blocks(list);
        		exit(exit_code);
			}
			if (access(last_input, W_OK) != 0)
			{
				// printf("burda\n");
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(last_input, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				list->dqm = 1;
				exit_code = list->dqm;
				free_pipe_blocks(list);
        		exit(exit_code);
			}
			if (fd < 0)
			{
				//printf("burda2\n");
				error_msg(i, list->acces_args->args[i]->str, 0, list);
				list->dqm = 1;
				exit_code = list->dqm;
				free_pipe_blocks(list);
        		exit(exit_code);
			}
			if(last_fd !=-1)
				close(last_fd);
			last_fd = fd;
		}
		else
		{
			error_msg(2, NULL, 3, list);
			exit_code = list->dqm;
			free_pipe_blocks(list);
        	exit(exit_code);
		}
	}
	if(last_fd!=-1)
	{
		dup2(last_fd, 1);
		close(last_fd);
	}
	//renew_block2(list);
}

void	handle_input(t_general *list, int i)
{
	int	fd;
	char *last_input;
	int last_fd;
	int exit_code;
	last_fd = -1;
	// while (list->acces_args->args[i])
	// {
		if (ft_strcmp(list->acces_args->args[i]->str, "<") == 0)
		{
			if (list->acces_args->args[i + 1])
			{
				i++;
				last_input = list->acces_args->args[i]->str;
				
				fd = open(last_input, O_RDONLY, 0644);
				if(access(last_input, F_OK) != 0)
				{
					error_msg(2, last_input, 0, list);
					exit_code = list->dqm;
					free_pipe_blocks(list);
        			exit(exit_code);
				}
				if (access(last_input, R_OK) != 0)
				{
					//printf("burda\n");
					ft_putstr_fd("bash: ", 2);
					ft_putstr_fd(last_input, 2);
					ft_putstr_fd(": Permission denied\n", 2);
					list->dqm = 1;
					exit_code = list->dqm;
					free_pipe_blocks(list);
        			exit(exit_code);
				}
				
				if (fd < 0)
				{
					error_msg(i, list->acces_args->args[i]->str, 0, list);
					list->dqm = 1;
					exit_code = list->dqm;
					free_pipe_blocks(list);
        			exit(exit_code);
				}
				if(last_fd !=-1)
					close(last_fd);
				last_fd = fd;
			}
			else
			{
				error_msg(2, NULL, 3, list);
                exit_code = list->dqm;
				free_pipe_blocks(list);
        		exit(exit_code);
			}
		}
	// 	i++;
	// }
	if(last_fd!=-1)
	{
		dup2(last_fd, 0);
		close(last_fd);
	}
	//renew_block2(list);
	
}
