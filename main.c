/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/01 17:55:33 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void free_split(char **str)
// {
//     int i;
//     if(!str)
//         return;
//     i = 0;
//     while(str[i])
//     {
//         free(str[i]);
//         i++;
//     }
//     free(str);
// }

// void exit_cmd(char **ar)
// {
//     if(ft_strncmp(ar[0], "exit", 4)==0)
//     {
//         free_split(ar);
//         exit(0);
//     }
// }

// void cd_cmd(char **ar)
// {
//     if(ft_strncmp(ar[0], "cd", 2)==0)
//     {
//         if(!ar[1])
//         {
//             char *line;
//             line  = getenv("HOME");
//             chdir(line);
//         }
//         else
//         {
//             chdir(ar[1]);
//         }
//     }
// }

// void pwd_cmd(char **ar)
// {
//     char *line;
//     if(ft_strncmp(ar[0],"pwd",3)==0)
//     {
//         line = getcwd(NULL, 0);
//         if(!line)
//         {
//             printf("Error\n");
//             free(line);
//             free_split(ar);
//             exit(1);
//         }
//         printf("%s\n", line);
//     }
// }

// void add_flag(t_general *a)
// {
//     int i = 1;
//     a->args[0].flag = 1;
//     while(a->args[i].str)
//     {
//         if (a->args[i].str[0] == '|')
//         {
//             a->args[i+1].flag = 1;
//             i++;
//         }
//         else if(a->args[i].str[0] == '"')
//             a->args[i].flag = 2;
//         else if(a->args[i].str[0] == '\'')
//             a->args[i].flag = 3;
//         else
//             a->args[i].flag = 0;
//         i++;
//     }
//     echo_cont(a);
// }
// void echo_cont(t_general *a)
// {
//     int i = 0;
//     while(a->args[i].str)
//     {
//         if(a->args[i].flag == 1)
//         {
//             if(ft_strncmp(a->args[i].str, "\"echo ", 6) == 0 || ft_strncmp(a->args[i].str, "\'echo '", 6) == 0)
//             {
//                 printf("Command 'echo ' not found, did you mean: command 'echo' from deb coreutils (8.32-4.1ubuntu1.2) Try: apt install <deb name>\n");
//                 return ;
//             }
//         }
//         i++;
//     }
//     dolar_control(a);
// }

t_arg *create_arg(const char *str, int flag)
{
    t_arg *arg = malloc(sizeof(t_arg));
    if (!arg)
        return NULL;
    arg->str = ft_strdup(str);
    arg->flag = flag;
    return arg;
}

t_pipeafter *create_pipeafter(void)
{
    t_pipeafter *pa = malloc(sizeof(t_pipeafter));
    if (!pa)
        return NULL;
    pa->args = malloc(sizeof(t_arg *) * 100); // max arg 100 (geliştirilebilir)
    if (!pa->args)
    {
        free(pa);
        return NULL;
    }
    return pa;
}

t_general *create_general_node(void)
{
    t_general *node = malloc(sizeof(t_general));
    if (!node)
        return NULL;
    node->acces_args = create_pipeafter();
    node->blocs = NULL;
    node->next = NULL;
    return node;
}

t_env *create_env_node(void)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->data = NULL;
    node->next = NULL;
    return node;
}

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


void dolar_control(t_general *dolar)
{
    while (dolar)
    {
        int i = 0;
        while (dolar->acces_args->args[i])
        {
           
            if ((dolar->acces_args->args[i]->flag == 0 || dolar->acces_args->args[i]->flag == 2)
                && ft_strchr(dolar->acces_args->args[i]->str, '$'))
            {
                char *control = ft_strchr(dolar->acces_args->args[i]->str, '$');
                control = ft_strtrim(control, "\" ");
                if (control && control[1] != ' ' && control[1] != '\0')
                {
                    control++;
                    char *b = getenv(control);
                    if (b)
                        printf("%s\n", b);
                }
                //free(control);
            }
            i++;
        }
        dolar = dolar->next;
    }
}


void pipe_parse(t_general **pipe_block, char *line)
{
    char **pipe_str = ft_split(line, '|');
    int i = 0;
    t_general *tmp = *pipe_block;

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
    free(pipe_str); // optional: split free ediliyorsa burada
}


void print_pipes(t_general *pipe_block)
{
    t_general *tmp;
    int pipe_num;

    if (!pipe_block)
        return;

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
                int i = 0;
                printf("  Arguments:\n");

                while (tmp->acces_args->args[i] && tmp->acces_args->args[i]->str)
                {
                    printf("    Arg %d: %s", i + 1, tmp->acces_args->args[i]->str);
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
void parse_input(t_general *a)
{
    int i, s, k;

    while (a)
    {
        i = 0;
        k = 0;

        while (a->blocs[i])
        {
            int flag;
            while (a->blocs[i] == ' ')
                i++;

            if (a->blocs[i] == '\0')
                break;

            if (a->blocs[i] == '"' || a->blocs[i] == '\'')
            {
                char quote = a->blocs[i++];
                s = i;
                while (a->blocs[i] && a->blocs[i] != quote)
                    i++;
                if (quote == '"')
                    flag = 0;
                else
                    flag = 1;
                if (!a->blocs[i])
                {
                    if(flag==0)
                        message(0);
                    else
                        message(1);
                }

                a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, s, i - s), flag);
                if (a->blocs[i] == quote)
                    i++;
            }
            else if (a->blocs[i] == '<')
            {
                if (a->blocs[i + 1] == '<')
                {
                    a->acces_args->args[k++] = create_arg("<<", 3);
                    i += 2;
                }
                else
                {
                    a->acces_args->args[k++] = create_arg("<", 4);
                    i++;
                }
            }
            else if (a->blocs[i] == '>')
            {
                if (a->blocs[i + 1] == '>')
                {
                    a->acces_args->args[k++] = create_arg(">>", 5);
                    i += 2;
                }
                else
                {
                    a->acces_args->args[k++] = create_arg(">", 6);
                    i++;
                }
            }
            else
            {
                s = i;
                while (a->blocs[i] && a->blocs[i] != ' ' && a->blocs[i] != '"' && a->blocs[i] != '\'')
                    i++;
                a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, s, i - s), 2);
            }
        }
        a->acces_args->args[k] = NULL;
        a = a->next;
    }
}

int main(int argc, char *argv[], char **envp)
{
    char *line;
    t_general *pipe_blocs = create_general_node();
    t_env *env;
    static int first_run = 1;
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
        dolar_control(pipe_blocs);
        check_cmd_built_in(pipe_blocs, &env, envp);
        pipe_blocs = create_general_node();
        free(line);
    }
}
