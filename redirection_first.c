/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_first.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:51:00 by bucolak           #+#    #+#             */
/*   Updated: 2025/06/22 15:00:16 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_args(t_arg **args)
// {
//     int i = 0;

//     printf("AYIKLANMIŞ ARGÜMANLAR:\n");
//     while (args && args[i])
//     {
//         printf("Arg[%d]: \"%s\", Flag: %d\n", i, args[i]->str,
//		args[i]->flag);
//         i++;
//     }
// }

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
	(*new)[*j] = malloc(sizeof(t_arg)); // burayı değiştirdim 13.51
	(*new)[*j]->str = ft_strdup(tmp->acces_args->args[*i]->str);
	(*new)[*j]->flag = tmp->acces_args->args[*i]->flag;
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
	new = malloc(sizeof(t_arg *) * (i + 1));
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
}

void	handle_output(t_general *list)
{
	int	i;
	int	fd;

	i = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, ">") == 0)
		{
			if (list->acces_args->args[i + 1])
			{
				i++;
				fd = open(list->acces_args->args[i]->str,
						O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (fd < 0)
					perror("open");
				dup2(fd, 1);
				close(fd);
                renew_block2(list);
				break ;
			}
			else
			{
				error_msg(2, NULL, 3, list);
				exit(list->dqm) ;
			}
		}
		i++;
	}
}

void	handle_input(t_general *list)
{
	int	i;
	int	fd;
	char *last_input;
	int last_fd;
	last_fd = -1;
	i = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<") == 0)
		{
			if (list->acces_args->args[i + 1])
			{
				i++;
				last_input = list->acces_args->args[i]->str;
				fd = open(last_input, O_RDONLY, 0644);
				if (fd < 0)
				{
					ft_putstr_fd("bash: ",2);
					ft_putstr_fd(last_input, 2);
					ft_putstr_fd(": No such file or directory\n", 2);
					list->dqm = 1;
					exit(list->dqm);
				}
				if(last_fd !=-1)
					close(last_fd);
				last_fd = fd;
			}
			else
			{
				error_msg(2, NULL, 3, list);
                exit(list->dqm);
			}
		}
		i++;
	}
	if(last_fd!=-1)
	{
		dup2(last_fd, 0);
		close(last_fd);
	}
	renew_block2(list);
	
}
