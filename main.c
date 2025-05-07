/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/08 01:49:05 by buket            ###   ########.fr       */
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

// void dolar_control(t_general *dolar)
// {
//     while (dolar)
//     {
//         int i = 0;
//         while (dolar->acces_args->args[i])
//         {
           
//             if ((dolar->acces_args->args[i]->flag == 0 || dolar->acces_args->args[i]->flag == 2)
//                 && ft_strchr(dolar->acces_args->args[i]->str, '$'))
//             {
//                 char *control = ft_strchr(dolar->acces_args->args[i]->str, '$');
//                 control = ft_strtrim(control, "\" ");
//                 if (control && control[1] != ' ' && control[1] != '\0')
//                 {
//                     control++;
//                     char *b = getenv(control);
//                     if (b)
//                         printf("%s\n", b);
//                 }
//                 //free(control);
//             }
//             else if((dolar->acces_args->args[i]->flag == 1) && ft_strchr(dolar->acces_args->args[i]->str, '$'))
//             {
//                 char *control = ft_strtrim(control, "'");
//                 printf("%s\n", control);
//             }
//             i++;
//         }
//         dolar = dolar->next;
//     }
// }

char *dolar_control(char *str)
{
        int i = 0;
        int k;
        char *control;
        while (str[i])
        {
            if (ft_strchr(str, '$'))
            {
                control = ft_strchr(str, '$');
                // control = ft_strtrim(control, "\"'");
                if (control && control[1] != ' ' && control[1] != '\0')
                {
                    k = i;
                    while(control[i] != '"' || control[i] != '\'')
                        i++;
                    i--;
                    control++;
                    char *b = getenv(ft_substr(control, k, i-k));
                    if (b)
                        printf("%s\n", b);
                }
                //free(control);
            }
            i++;
        }
        return control;
}

void pipe_parse(t_general **pipe_block, char *line)
{
    int i = 0;
    t_general *tmp;
    tmp = *pipe_block;
    while(line[i] != '"' && line[i] != '\'')
        i++;
    if(line[i] && line[i] != '"' && line[i] != '\'')
    {
        printf("%c\n",line[0]);
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
            while (a->blocs[i] == ' ')
                i++;
            if (a->blocs[i] == '\0')
                break;
            s = i;
            int in_quotes = 0;
            char current_quote = 0;
            int flag = 2; // Normal durum, tırnak yok
            
            // İlk karakterin tırnak olup olmadığını kontrol et
            char starting_quote = 0;
            if (a->blocs[i] == '"' || a->blocs[i] == '\'')
                starting_quote = a->blocs[i];
            
            // Argümanın sınırlarını bul
            while (a->blocs[i] && (in_quotes || a->blocs[i] != ' '))
            {
                // Eğer tırnak içindeysek, tüm karakterleri normal karakter olarak ele al
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
            
            // Tırnak kapanmadı kontrolü ve flag belirleme
            if (in_quotes)
            {
                // Asıl sorun burada olabilir. Tırnak kapanmadı hatası vermek yerine
                // varsayılan olarak kapanmış gibi devam edelim
                in_quotes = 0;
            }
            
            // Flag türünü belirle
            int length = i - s;
            if (length >= 2 && a->blocs[s] == starting_quote && a->blocs[i-1] == starting_quote && starting_quote != 0)
            {
                // Tamamen tırnak içinde
                flag = (starting_quote == '"') ? 0 : 1; // Çift tırnak için 0, tek tırnak için 1
            }
            else if (starting_quote != 0 || (length > 0 && (a->blocs[s] == '"' || a->blocs[s] == '\'' || 
                                            a->blocs[i-1] == '"' || a->blocs[i-1] == '\'')))
            {
                // Karışık içerik (tırnak ile başlıyor/bitiyor ama tam kapanmıyor veya başka tırnaklar da var)
                flag = 4;
            }
            
            // Eğer pipe işaretini bir argüman içinde görüyorsak ve tırnak içindeysek, normal karakter olarak ele al
            a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, s, length), flag);
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
        //dolar_control(pipe_blocs);
        check_cmd_built_in(pipe_blocs, &env, envp);
        //check_cmd_sys_call(pipe_blocs, &env); 
        pipe_blocs = create_general_node();
        free(line);
    }
}
