/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/06 17:20:27 by buket            ###   ########.fr       */
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
        print_pipes(pipe_blocs);
        dolar_control(pipe_blocs);
        //check_cmd_built_in(pipe_blocs, &env, envp);
        //check_cmd_sys_call(pipe_blocs, &env); 
        pipe_blocs = create_general_node();
        free(line);
    }
}
