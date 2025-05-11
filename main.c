/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/11 20:20:35 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void free_split(char **str)
// {
//     int i;
//     if(!str)
//         return ;
//     i = 0;
//     while(str[i])
//     {
//         free(str[i]);
//         i++;
//     }
//     free(str);
// }

void	pipe_parse(t_general **pipe_block, char *line)
{
	int			i;
	t_general	*tmp;
	char		**pipe_str;

	i = 0;
	tmp = *pipe_block;
	while (line[i] && line[i] != '"' && line[i] != '\'')
		i++;
	if (line[i] && line[i] != '"' && line[i] != '\'')
	{
		pipe_str = ft_split(line, '|');
		i = 0;
		while (pipe_str[i])
		{
			if (pipe_str[i][0] != '\0')
			{
				tmp->blocs = ft_strtrim(pipe_str[i], " ");
				if (pipe_str[i + 1])
				{
					tmp->next = create_general_node();
					tmp = tmp->next;
				}
			}
			i++;
		}
	}
	else
	{
		tmp->blocs = ft_strdup(line);
	}
}

void	print_pipes(t_general *pipe_block)
{
	t_general	*tmp;
	int			pipe_num;
	int			i;

	if (!pipe_block)
		return ;
	tmp = pipe_block;
	pipe_num = 1;
	printf("\n--- Parsed Pipes ---\n");
	while (tmp)
	{
		if (tmp->blocs)
		{
			printf("Pipe %d: [%s]\n", pipe_num, tmp->blocs);
			if (tmp->acces_args && tmp->acces_args->args)
			{
				i = 0;
				printf("  Arguments:\n");
				while (tmp->acces_args->args[i]
					&& tmp->acces_args->args[i]->str)
				{
					printf("    Arg %d: %s", i + 1,
							tmp->acces_args->args[i]->str);
					printf(" (flag: %d)\n", tmp->acces_args->args[i]->flag);
					i++;
				}
			}
			pipe_num++;
		}
		tmp = tmp->next;
	}
	printf("-------------------\n");
}
void	parse_input(t_general *a)
{
	int		len;
	int		in_quotes;
	char	current_quote;
	char	starting_quote;
	int		length;

	int i, s, k;
	while (a)
	{
		i = 0;
		k = 0;
		while (a->blocs[i])
		{
			// Boşlukları atla
			while (a->blocs[i] == ' ')
				i++;
			if (a->blocs[i] == '\0')
				break ;
			// Redirection operatorleri kontrolü
			if (a->blocs[i] == '<' || a->blocs[i] == '>')
			{
				len = 1;
				if (a->blocs[i] == a->blocs[i + 1]) // << veya >>
					len = 2;
				a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, i,
							len), 5); // flag 5: redirection
				i += len;
				continue ;
			}
			// Pipe operatorü kontrolü
			if (a->blocs[i] == '|')
			{
				a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, i, 1),
														6); // flag 6: pipe
				i++;
				continue ;
			}
			// Argüman başı
			s = i;
			in_quotes = 0;
			current_quote = 0;
			starting_quote = 0;
			int flag = 2; // varsayılan: normal argüman
			// İlk karakter tırnak mı?
			if (a->blocs[i] == '"' || a->blocs[i] == '\'')
			{
				starting_quote = a->blocs[i];
				in_quotes = 1;
				current_quote = a->blocs[i];
				i++;
			}
			// Argüman uzunluğu belirleme
			while (a->blocs[i])
			{
				if (!in_quotes && (a->blocs[i] == ' ' || a->blocs[i] == '<'
						|| a->blocs[i] == '>' || a->blocs[i] == '|'))
					break ;
				if (a->blocs[i] == '"' || a->blocs[i] == '\'')
				{
					if (in_quotes && a->blocs[i] == current_quote)
					{
						in_quotes = 0;
						current_quote = 0;
					}
					else if (!in_quotes)
					{
						in_quotes = 1;
						current_quote = a->blocs[i];
					}
				}
				i++;
			}
			length = i - s;
			// Flag belirleme
			if (starting_quote != 0 && a->blocs[i - 1] == starting_quote)
			{
				// düzgün kapatılmış tırnak
				flag = (starting_quote == '"') ? 0 : 1;
				s++;
				length -= 2;
			}
			else if (starting_quote != 0)
			{
				// kapatılmamış tırnak
				flag = 4;
			}
			else
			{
				// dış tırnak yok ama içeride tırnak varsa -> karmaşık
				for (int j = s; j < s + length; j++)
				{
					if (a->blocs[j] == '"' || a->blocs[j] == '\'')
					{
						flag = 4;
						break ;
					}
				}
			}
			a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, s,
						length), flag);
		}
		a->acces_args->args[k] = NULL;
		a = a->next;
	}
}

int	has_redireciton(t_general *pipe_blocks)
{
	int	i;

	i = 0;
	while (pipe_blocks->acces_args->args[i])
	{
		if (ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<") == 0
			|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<<") == 0
			|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">") == 0
			|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">>") == 0)
			return (1);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[], char **envp)
{
	char		*line;
	t_general	*pipe_blocs;
	t_env		*env;
	static int	first_run;

	(void)argc;
	(void)argv;
	pipe_blocs = create_general_node();
	first_run = 1;
	env = create_env_node();
	if (first_run)
	{
		get_env(&env, envp);
		first_run = 0;
	}
	while (1)
	{
		line = readline("Our_shell% ");
		if (!line)
		{
			printf("Error\n");
			exit(1);
		}
		add_history(line);
		pipe_parse(&pipe_blocs, line);
		parse_input(pipe_blocs);
		//print_pipes(pipe_blocs);
		while (pipe_blocs)
		{
			if (pipe_blocs->acces_args->args[0])
			{
				if ((!has_redireciton(pipe_blocs)
						&& is_built_in(pipe_blocs->acces_args->args[0]->str)))
				{
					check_cmd_built_in(pipe_blocs, &env);
				}
				else
				{
					check_cmd_sys_call(pipe_blocs, &env);
				}
			}
			pipe_blocs = pipe_blocs->next;
		}
		pipe_blocs = create_general_node();
		free(line);
	}
}
