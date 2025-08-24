/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_third.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:03:07 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 13:03:34 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key(char *str)
{
	int		i;
	int		start;
	char	*key;
	int		end;
	char	*final_key;

	i = 0;
	start = 0;
	while (str[i] && str[i] != '=')
		i++;
	end = i;
	key = ft_substr(str, start, end - start);
	final_key = ft_strtrim(key, "'\"");
	free(key);
	return (final_key);
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
	if (str[i] == '\0') // export a= durumu
		return (ft_strdup(""));
	if ((str[i] == '"' || str[i] == '\''))
		i++;
	k = i;
	while (str[i] && str[i])
		i++;
	if (str[i - 1] == '"' || str[i] == '\'')
		i--;
	return (ft_substr(str, k, i - k));
}
