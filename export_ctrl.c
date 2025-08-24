/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_ctrl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:08:45 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 14:11:35 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *name)
{
	int	i;

	i = 1;
	if (!name || !*name)
		return (0);
	if (!ft_isalpha(*name) && *name != '_')
	{
		return (0);
	}
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	ctrl_is_valid_identifier_builtin_helper(t_general *tmp, int i)
{
	if (is_valid_identifier(tmp->acces_args->args[i + 1]->str) == 0)
	{
		ft_putstr_fd("bash: export: '", 2);
		ft_putstr_fd(tmp->acces_args->args[i + 1]->str, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		tmp->dqm = 1;
		return (1);
	}
	return (0);
}

void	export_cntrl_builtin_helper(t_general *tmp, int i, int *c, t_full *full)
{
	int	j;
	int	k;

	k = i + 1;
	while (tmp->acces_args->args[k])
	{
		j = 0;
		while (tmp->acces_args->args[k]->str[j])
		{
			if (tmp->acces_args->args[k]->str[j] == '=')
			{
				tmp->acces_args->args[k]->env_flag = 1;
				break ;
			}
			(*c)++;
			j++;
		}
		k++;
	}
	if (*c > 0)
		full->new = malloc(sizeof(char *) * ((*c) + 1));
	if (!full->new)
		return ;
}

void	export_cntrl_builtin_helper_scnd(t_full *full, t_general *tmp)
{
	int	k;
	int	equals_index;
	int	j;

	j = 0;
	k = 0;
	while (tmp->acces_args->args[k])
	{
		if (tmp->acces_args->args[k]->env_flag == 1)
		{
			equals_index = 0;
			while (tmp->acces_args->args[k]->str[equals_index]
				&& tmp->acces_args->args[k]->str[equals_index] != '=')
				equals_index++;
			full->new[j++] = ft_substr(tmp->acces_args->args[k]->str, 0,
					equals_index);
		}
		k++;
	}
	full->new[j] = NULL;
}

void	put_flag_builtin_helper(t_full *full)
{
	int		k;
	t_env	*env;

	env = full->node;
	while (env)
	{
		k = 0;
		while (full->new[k])
		{
			if (ft_strcmp(full->new[k], env->key) == 0)
				env->has_equal = 1;
			k++;
		}
		env = env->next;
	}
}
