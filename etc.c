/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:30:26 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/11 15:59:04 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lsttsize(t_env *lst)
{
	size_t	i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

t_env	*ft_lsttlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

int is_numeric(char *str)
{
    int i;
    i = 0;
    int k = 0;
    while(str[i])
    {
        if(ft_isdigit(str[i]) == 1)
            k++;
        i++;
    }
    if((size_t)k == ft_strlen(str))
        return 1;
    return 0;
}

int is_repeated(t_env **node, char *ky, char *dt)
{
    t_env *tmp = *node;
    while(tmp)
    {
        if(ft_strcmp(ky, tmp->key) == 0)
        {
            tmp->data = dt;
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}