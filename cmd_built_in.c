/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/08/11 18:09:33 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in_helper_func_2(t_full *full, int i)
{
	if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
					"pwd") == 0)
		pwd_cmd(&full->pipe_blocks->acces_args->args[i]->str, full->pipe_blocks, full->node);
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
		"unset") == 0)
		{
			if(!full->pipe_blocks->acces_args->args[i + 1])
				return;
			unset_cmd(full->pipe_blocks, &full->node);
		}
		
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
						"env") == 0)
		print_env(full->pipe_blocks, &full->node, i);
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
						"exit") == 0)
		exit_cmd(full);
}

int is_valid_identifier(const char *name) {

	int i;
	i = 1;
    if (!name || !*name)
        return 0;
    
    // İlk karakter kontrolü
    if (!ft_isalpha(*name) && *name != '_')
        return 0;
    
    // Geri kalan karakterler kontrolü
	while(name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
            return 0;
		i++;
	}
    
    return 1;
}

void	built_in_helper_func(t_full *full, int i)
{
	t_general *tmp =full->pipe_blocks;
	t_env *env = full->node;
	int c = 0;
	int j;
	int k;
	if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
						"export") == 0)
	{
		if ((full->pipe_blocks->acces_args->args[i + 1]
				&& full->pipe_blocks->acces_args->args[i + 1]->str))
			{
				k=i+1;
				if(is_valid_identifier(full->pipe_blocks->acces_args->args[i + 1]->str) == 0)
				{
					ft_putstr_fd("bash: export: ", 2);
					ft_putstr_fd("`=': not a valid identifier\n", 2);
					full->pipe_blocks->dqm=1;
				}
				else
				{
					while(tmp->acces_args->args[k])
					{
						j = 0;
						while(tmp->acces_args->args[k]->str[j])
						{
							if(tmp->acces_args->args[k]->str[j]=='=')
							{
								c++;
								tmp->acces_args->args[k]->env_flag = 1;
								break;
							}
							j++;
						}
						k++;
					}
					if(c>0)
						full->new = malloc(sizeof(char *) * (c+1));
					if(!full->new)
						return ;
					tmp =full->pipe_blocks;
					k = 0;
					j = 0;
					while(tmp->acces_args->args[k])
					{

						if(tmp->acces_args->args[k]->env_flag == 1)
						{
							int equals_index = 0;
							while (tmp->acces_args->args[k]->str[equals_index] && tmp->acces_args->args[k]->str[equals_index] != '=')
							{
							    equals_index++;
							}
							full->new[j++] = ft_substr(tmp->acces_args->args[k]->str, 0, equals_index);
						}
						k++;
					}
					full->new[j] = NULL;
					create_env(full->pipe_blocks, &full->node);
					while(env)
					{
						k = 0;
						while(full->new[k])
						{
							if(ft_strcmp(full->new[k], env->key) == 0)
							{
								env->has_equal = 1;
							}
							k++;
						}	
						env = env->next;
					}
				}
			}
		else
		{
			print_export_env(&full->node, full->pipe_blocks);
		}
	}
	built_in_helper_func_2(full, i);
	//free_split(new);
}

void	check_cmd_built_in(t_general *pipe_blocs, t_env **node, t_pipe *pipe, t_now *get)
{
	int	i;
	t_full full;
	
	full.pipe_blocks = pipe_blocs;
	full.node = *node;
	full.get = get;
	full.pipe = pipe;
	full.new = NULL;
	i = 0;
	while (pipe_blocs->acces_args->args[i])
	{
		if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "cd") == 0)
		{
			cd_cmd(pipe_blocs->acces_args->args, *node, pipe_blocs);
			break ;
		}
		else if (ft_strcmp(full.pipe_blocks->acces_args->args[0]->str,
						"$?") == 0)
		{
			ft_putstr_fd(ft_itoa(full.pipe_blocks->dqm), 2);
			ft_putstr_fd(": command not found\n", 2); //1
			full.pipe_blocks->dqm = 127;
			break;
		}
		built_in_helper_func(&full, i);
		if (ft_strcmp(pipe_blocs->acces_args->args[i]->str,
						"echo") == 0)
		{
							initalized_echo(pipe_blocs);
							break;
		}
		i++;
	}
	*node = full.node;
}

void cd_helper(t_arg **args, char *env_name, t_general *pipe_blocks, t_env *env)
{
	t_env *tmp = env;
	if (args[1]->str[0] == '$' && (args[1]->flag == 0
				|| args[1]->flag == 2))
	{
		env_name = args[1]->str + 1; // bi sorun çıkarsa buraya bi gözat
    	chdir(get_getenv(env ,env_name));
		return ;
	}
	else if(chdir(args[1]->str)==-1)
	{
		perror("cd");
		pipe_blocks->dqm = 1;
	}
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") == 0)
		{
			free(tmp->data);
			tmp->data= ft_strdup(get_getenv(env, "PWD")); //BAK!! Fonksiyon otomatik olarak yeterli boyutta bellek allocate eder (malloc ile)
			break;
		}
		tmp = tmp->next;
	}
	 tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PWD") == 0)
		{
			free(tmp->data);
			tmp->data = getcwd(NULL, 0);
			break;
		}
		tmp = tmp->next;
	}
}

void	cd_cmd(t_arg **args, t_env *env, t_general *pipe_blocks)
{
	char	*line;
	char *env_name;
	t_env *tmp;
	tmp = env;
	
	env_name = NULL;
	if (!args[1] || ft_strcmp(args[1]->str, "~") == 0)
	{
		line = get_getenv(env, "HOME");
		if (line)
			chdir(line);
	}
	else if(ft_strcmp(args[1]->str, "-") == 0)
	{
		char *old_pwd = get_getenv(env, "OLDPWD");
		char *pwd = get_getenv(env, "PWD");
		char *new_oldpwd = ft_strdup(pwd);
		while(tmp)
		{
			if(ft_strcmp(tmp->key,"OLDPWD") == 0)
			{
				if(tmp->data)
					free(tmp->data);
				tmp->data = new_oldpwd;
			}
			else if(ft_strcmp(tmp->key,"PWD") == 0)
			{
				if(tmp->data)
					free(tmp->data);
				tmp->data = ft_strdup(old_pwd);
			}
			tmp = tmp->next;
		}
		ft_putstr_fd(new_oldpwd, 1);
		ft_putchar_fd('\n',1);
		chdir(get_getenv(env, "OLDPWD"));
	}
	else if(args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n",2);
		pipe_blocks->dqm = 1;
	}
	else
		cd_helper(args, env_name, pipe_blocks, env);
}

void	pwd_cmd(char **ar, t_general *list, t_env *env)
{
	char	*line;

	if (ft_strncmp(ar[0], "pwd", 3) == 0)
	{
		line = getcwd(NULL, 0);
		list->dqm = 0;
		if (!line)
		{
			printf("Error\n");	line = get_getenv(env, "HOME");
			free(line);
			list->dqm = 1;
			return ;
		}
		printf("%s\n", line);
		free(line);
		list->dqm = 0;
	}
}
