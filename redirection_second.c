/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/06/02 20:41:27 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_heredoc(t_general *list)
{
	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	int		i;
	int		j;
	t_arg	**new_arg;

	i = 0;
	while (list->acces_args->args[i])
	{
		i++;
	}
	new_arg = malloc(sizeof(t_arg *) * (i + 1));
	i = 0;
	j = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
		{
			i += 2; // skip '<<' and its delimiter
			continue ;
		}
		new_arg[j++] = list->acces_args->args[i++];
	}
	new_arg[j] = NULL;
	list->acces_args->args = new_arg;
}

int count_heredoc(t_general *list)
{
	int i = 0;
	int j = 0;
	int c = 0;
	if (!list || !list->acces_args || !list->acces_args->args)
		return 0;
	while(list->acces_args->args[i])
	{
		if(ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			c++;
		i++;
	}
	list->limiter = malloc(sizeof(char *)*(c +1));
	i = 0;
	
	while(list->acces_args->args[i])
	{
		if(ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
		{
			if(list->acces_args->args[i+1])
			{
				list->limiter[j] = ft_strdup(list->acces_args->args[i+1]->str);
				j++;
			}
		}
		i++;
	}
	list->limiter[j] = NULL;
	return c;
}

void	handle_heredoc(t_general *list)
{
	int		i;
	int		fd[2];
	char	*line;
	int		original_stdin;

	while (list)
	{
		count_heredoc(list); // limiter[] dizisini doldurur
		i = 0;
		//int limiter_index = 0;
		while (list->acces_args->args[i])
		{
			if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			{
				if (!list->acces_args->args[i + 1])
				{
					ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
					return ;
				}
				// get limiter
				char *limiter = list->acces_args->args[i + 1]->str;
				pipe(fd);
				while (1)
				{
					line = readline("heredoc > ");
					if (!line || ft_strcmp(line, limiter) == 0)
					{
						free(line);
						break;
					}
					ft_putstr_fd(line, fd[1]);
					ft_putstr_fd("\n", fd[1]);
					free(line);
				}
				close(fd[1]); // close writing end

				// son heredoc ise stdin yönlendir
				if (!list->acces_args->args[i + 2] || ft_strcmp(list->acces_args->args[i + 2]->str, "<<") != 0)
				{
					original_stdin = dup(STDIN_FILENO);
					dup2(fd[0], STDIN_FILENO);
					list->heredoc_fd = original_stdin;
				}
				close(fd[0]);
				i += 2; // skip '<<' and its delimiter
				continue;
			}
			i++;
		}
		remove_heredoc(list);
		list = list->next;
	}
}



// void	remove_heredoc(t_general *list)
// {
// 	int		i;
// 	int		j;
// 	t_arg	**new_arg;

// 	i = 0;
// 	while (list->acces_args->args[i])
// 	{
// 		i++;
// 	}
// 	new_arg = malloc(sizeof(t_arg *) * (i + 1));
// 	i = 0;
// 	j = 0;
// 	while (list->acces_args->args[i])
// 	{
// 		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
// 		{
// 			i += 2; // skip '<<' and its delimiter
// 			continue ;
// 		}
// 		new_arg[j++] = list->acces_args->args[i++];
// 	}
// 	new_arg[j] = NULL;
// 	list->acces_args->args = new_arg;
// }

// void	handle_heredoc(t_general *list)
// {
// 	int		i;
// 	char	*line;
// 	int		fd[2];

// 	i = 0;
// 	while (list)
// 	{
// 		i = 0;
// 		while (list->acces_args->args[i])
// 		{
// 			if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
// 			{
// 				if (!list->acces_args->args[i + 1])
// 				{
// 					ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
// 									2);
// 					return ;
// 				}
// 				i++;
// 				pipe(fd);
// 				while (1)
// 				{
// 					line = readline(">");
// 					if (ft_strcmp(line, list->acces_args->args[i]->str) != 0)
// 					{
// 						ft_putendl_fd(line, fd[1]);
// 					}
// 					else
// 						break ;
// 				}
// 				close(fd[1]);
// 				// if (list->heredoc_fd != -1)
// 				// 	close(list->heredoc_fd);
// 				// list->heredoc_fd = fd[0];
// 			}
// 			i++;
// 		}
// 		remove_heredoc(list);
// 		list = list->next;
// 	}
// }
