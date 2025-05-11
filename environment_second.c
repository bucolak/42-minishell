/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:19:44 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/11 16:32:57 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key(char *str)
{
	int		i;
	int		start;
	char	*key;
	int		end;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			printf("error\n");
			return (NULL);
		}
		i++;
	}
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	end = i;
	if (str[i] == '=')
		end = i + 1;
	key = ft_substr(str, start, end - start);
	return (ft_strtrim(key, "'\""));
}

char	*get_data(char *str)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	if ((str[i] == '"' || str[i] == '\''))
		i++;
	k = i;
	while (str[i] && str[i])
		i++;
	if (str[i - 1] == '"' || str[i] == '\'')
		i--;
	return (ft_substr(str, k, i - k));
}

void	print_export_env(t_env **env)
{
	t_env	**new_env;
	t_env	*node;
	int		i;
	int		len;

	new_env = export_cmd(env);
	i = 0;
	len = ft_lsttsize(*env);
	while (i < len)
	{
		node = new_env[i];
		if (node->key)
		{
			if (node->key && node->data && node->data[0] != '\0')
				printf("declare -x %s\"%s\"\n", node->key, node->data);
			else if (node->key)
				printf("declare -x %s%s\n", node->key, node->data);
		}
		i++;
	}
}

void	export_cmd_helper_func(t_env **env, t_env **new_env, t_env *swap,
		int *j)
{
	int	i;
	int	len;

	len = ft_lsttsize(*env);
	i = 0;
	while (i < len - 1)
	{
		*j = 0;
		while (*j < len - 1)
		{
			if (ft_strcmp(new_env[*j]->key, new_env[*j + 1]->key) > 0)
			{
				swap = new_env[*j];
				new_env[*j] = new_env[*j + 1];
				new_env[*j + 1] = swap;
			}
			j++;
		}
		i++;
	}
}

t_env	**export_cmd(t_env **env)
{
	int		i;
	int		j;
	int		len;
	t_env	**new_env;
	t_env	*tmp;
	t_env	*swap;

	swap = NULL;
	len = ft_lsttsize(*env);
	i = 0;
	new_env = env;
	new_env = malloc(sizeof(t_env *) * len);
	tmp = *env;
	while (i < len)
	{
		new_env[i] = tmp;
		tmp = tmp->next;
		i++;
	}
	export_cmd_helper_func(env, new_env, swap, &j);
	return (new_env);
}
