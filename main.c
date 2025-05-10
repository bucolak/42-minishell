/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/10 19:08:29 by bucolak          ###   ########.fr       */
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

void pipe_parse(t_general **pipe_block, char *line)
{
    int i = 0;
    t_general *tmp;
    tmp = *pipe_block;
    while (line[i] && line[i] != '"' && line[i] != '\'')
        i++;
    if(line[i] && line[i] != '"' && line[i] != '\'')
    {
        char **pipe_str = ft_split(line, '|');
        
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
            // Boşlukları atla
            while (a->blocs[i] == ' ')
                i++;
            if (a->blocs[i] == '\0')
                break;

            // Redirection operatorleri kontrolü
            if (a->blocs[i] == '<' || a->blocs[i] == '>')
            {
                int len = 1;
                if (a->blocs[i] == a->blocs[i + 1]) // << veya >>
                    len = 2;
                a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, i, len), 5); // flag 5: redirection
                i += len;
                continue;
            }

            // Pipe operatorü kontrolü
            if (a->blocs[i] == '|')
            {
                a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, i, 1), 6); // flag 6: pipe
                i++;
                continue;
            }

            // Argüman başı
            s = i;
            int in_quotes = 0;
            char current_quote = 0;
            char starting_quote = 0;
            int flag = 2; // normal argüman

            // İlk tırnak karakterini kontrol et
            if (a->blocs[i] == '"' || a->blocs[i] == '\'')
            {
                starting_quote = a->blocs[i];
                in_quotes = 1;
                current_quote = a->blocs[i];
                i++;
            }

            while (a->blocs[i])
            {
                if (!in_quotes && (a->blocs[i] == ' ' || a->blocs[i] == '<' || a->blocs[i] == '>' || a->blocs[i] == '|'))
                    break;

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

            int length = i - s;

            // Argüman içinde tırnaklı ifade varsa flag = 4
            int quote_count = 0;
            for (int j = s; j < i; j++)
            {
                if (a->blocs[j] == '"' || a->blocs[j] == '\'')
                    quote_count++;
            }

            if (length >= 2 &&
                a->blocs[s] == starting_quote &&
                a->blocs[i - 1] == starting_quote &&
                starting_quote != 0 &&
                quote_count == 2)
            {
                // düzgün çift tırnaklı ifade
                flag = (starting_quote == '"') ? 0 : 1;
                s++;            // tırnak dahil etme
                length -= 2;
            }
            else if (quote_count > 0)
            {
                flag = 4; // içinde tırnak geçen ama düzgün kapatılmamış ya da karışık tırnaklı
            }

            a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, s, length), flag);
        }

        a->acces_args->args[k] = NULL;
        a = a->next;
    }
}


int main(int argc, char *argv[], char **envp)
{
    (void)argc;
    (void)argv;
    char *line;
    int i;
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
        while(pipe_blocs)
        {
            i = 0;
            while(pipe_blocs->acces_args->args[i])
            {
                if(ft_strcmp(pipe_blocs->acces_args->args[i]->str,"echo") == 0
                    || ft_strcmp(pipe_blocs->acces_args->args[i]->str,"cd") == 0
                    || ft_strcmp(pipe_blocs->acces_args->args[i]->str,"pwd") == 0
                    || ft_strcmp(pipe_blocs->acces_args->args[i]->str,"export") == 0
                    || ft_strcmp(pipe_blocs->acces_args->args[i]->str,"env") == 0
                    || ft_strcmp(pipe_blocs->acces_args->args[i]->str,"unset") == 0
                    || ft_strcmp(pipe_blocs->acces_args->args[i]->str,"exit") == 0)
                    {
                        check_cmd_built_in(pipe_blocs, &env);
                        //break;
                    }
                else
                    check_cmd_sys_call(pipe_blocs, &env);  
                i++;
            }
            pipe_blocs = pipe_blocs->next;
        }
        pipe_blocs = create_general_node();
        free(line);
    }
}
