#include "minishell.h"

// Free all memory associated with a t_arg structure
void free_arg(t_arg *arg) {
    if (arg) {
        if (arg->str)
            free(arg->str);
        free(arg);
    }
}

// Free all memory associated with a t_pipeafter structure
void free_pipeafter(t_pipeafter *pa) {
    if (pa) {
        if (pa->args) {
            for (int i = 0; pa->args[i]; i++) {
                free_arg(pa->args[i]);
            }
            free(pa->args);
        }
        free(pa);
    }
}

// Free all memory associated with a t_general node
void free_general_node(t_general *node) {
    if (node) {
        if (node->acces_args)
            free_pipeafter(node->acces_args);
        if (node->blocs)
            free(node->blocs);
        if (node->next)
            free_general_node(node->next);
        free(node);
    }
}

// Free all memory associated with t_now structure
void free_now(t_now *get) {
    if (get) {
        if (get->envp)
            free(get->envp);
        free(get);
    }
}

// Clean up all resources before program exit
void cleanup_all(t_general *pipe_blocs, t_env *env, t_now *get) {
    if (pipe_blocs)
        free_general_node(pipe_blocs);
    if (env)
        free_env(env);
    if (get)
        free_now(get);
}
