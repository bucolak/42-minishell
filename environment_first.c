/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_first.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:58:25 by buket             #+#    #+#             */
/*   Updated: 2025/08/05 20:09:55 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_env_helper_func(int *i, int *j, t_env *tmp, char **envp)
{
	(*i)++;
	if (tmp->key)
	{
		free(tmp->key);
	}
        
    if (tmp->data)
	{
		free(tmp->data);
	}
	if (envp[*j][*i] == '=')
	{
		tmp->key = ft_substr(envp[*j], 0, *i + 1);
		tmp->data = ft_strdup(envp[*j] + *i + 1);
	}
	else
	{
		tmp->key = ft_substr(envp[*j], 0, *i);
		tmp->data = NULL;
	}
}

void	get_env(t_env **node, char **envp)
{
	static int	j;
	int			i;
	t_env		*tmp;

	j = 0;
	i = 0;
	tmp = *node;
	while (envp[j])
	{
		i = 0;
		while (envp[j][i] != '=')
			get_env_helper_func(&i, &j, tmp, envp);
		j++;
		if (envp[j])
		{
			tmp->next = create_env_node();
			tmp = tmp->next;
		}
	}
}

void	print_env(t_general *list, t_env **node, int i)
{
	t_env	*tmp;

	if (list->acces_args->args[i + 1])
	{
		printf("Error\n");
		return ;
	}
	tmp = *node;
	while (tmp)
	{
		if (tmp->key && tmp->data)
		{
			if (ft_strchr(tmp->key, '='))
				printf("%s%s\n", tmp->key, tmp->data);
		}
		tmp = tmp->next;
	}
	list->dqm = 0;
}

int	key_cont(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!(key[i] >= 65 && key[i] <= 90) && !(key[i] >= 97 && key[i] <= 122)
			&& key[i] != 61 && key[i] != 95)
			return (0);
		i++;
	}
	return (1);
}
void print_message(char *key, char *data, t_general *list)
{
	ft_putstr_fd("bash: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd(data, 2);
	ft_putstr_fd("' : not a valid identifier\n", 2);
	list->dqm = 1;
}
void	ft_envadd_back(t_env **lst, char *key, char *data, t_general *list)
{
	t_env	*last;
	t_env	*new_node;
	t_env	*tmp;
	tmp = *lst;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            if (tmp->data)
                free(tmp->data);
            tmp->data = ft_strdup(data);
            list->dqm = 0;
            return;
        }
        tmp = tmp->next;
    }
	
	new_node = create_env_node();
	if (key)
	{
		if(key_cont(key) == 1)
		{
			new_node->key = ft_strdup(key);
			new_node->data = ft_strdup(data);			
			if (*lst == NULL)
				*lst = new_node;
			else
			{
				last = *lst;
				while (last->next)
					last = last->next;
				last->next = new_node;
			}
			list->dqm = 0;
		}
		else
			print_message(key, data, list);
	}
}

void create_env_2(t_general *list, t_env **env, int i)
{
	char	*new;
	char	*key;
    char	*data;
	
	new = list->acces_args->args[i]->str;
	if (list->acces_args->args[i] && new)
	{
		list->dqm = 0;
		key = get_key(new);
        data = get_data(new);
		//printf("%s\n", data);
		if ((count_dquote(new) % 2 == 0 || count_squote(new)
		% 2 == 0)) //burda is_repeat fonksiyonu vardı kaldırdım  çünkü şuan böyle gerekti sonra lazım olursa duruma göre bakarız
				{
					ft_envadd_back(env, key, data, list);
				}
        free(key);
        free(data);
	}
	if(list->acces_args->args[i+1] && ft_strcmp((*env)->key, " =")==0)
	{
		free(new);
		ft_putstr_fd("bash: export: ", 2);
		ft_putstr_fd("`=': not a valid identifier\n", 2);
	}
}

void	create_env(t_general *list, t_env **env)
{
	int		i;
	

	while (list)
	{
		i = 0;
		while (list->acces_args->args[i])
		{
			if (ft_strncmp(list->acces_args->args[i]->str, "export", 6) == 0)
			{
				i++;
				while (list->acces_args->args[i])
                {
                    create_env_2(list, env, i);
                    i++;
                }
                break;
			}
			i++;
		}
		list = list->next;
	}
}
