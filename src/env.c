/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:38:11 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 18:38:11 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **export(char **env, const char *new_var);
char **copyenv(char **envp);
void free_env(char **env);

char **copyenv(char **envp) { //ringraziare chattone che come lui nessuno
    int count = 0;

    // Conta le variabili nell'env originale
    while (envp[count])
        count++;

    // Alloca spazio per la copia
    char **env = malloc(sizeof(char *) * (count + 1));
    if (!env)
        return NULL;

    // Copia ogni stringa
    for (int i = 0; i < count; i++) {
        env[i] = ft_strdup(envp[i]);
        if (!env[i]) {
            // Libera la memoria in caso di errore
            while (i > 0)
                free(env[--i]);
            free(env);
            return NULL;
        }
    }
    env[count] = NULL; // Termina con NULL
    return env;
}

void free_env(char **env) {
    int i = 0;

    if (!env)
        return;

    while (env[i]) {
        free(env[i]);
        i++;
    }
    free(env);
}

char **export(char **env, const char *new_var) {
    if (!new_var || strchr(new_var, '=') == NULL) {
        int i = 0;
        while (env[i]) {
            printf("declare -x %s\n", env[i]);
            i++;
        }
        return env;
    }

    int i = 0;
    int name_len = strchr(new_var, '=') - new_var;

    // Cerca una variabile con lo stesso nome
    while (env[i]) {
        if (strncmp(env[i], new_var, name_len) == 0 && env[i][name_len] == '=') {
            free(env[i]);
            env[i] = strdup(new_var);
            return env;
        }
        i++;
    }

    // Aggiunge una nuova variabile
    char **nenv = realloc(env, sizeof(char *) * (i + 2));
    if (!nenv)
        return NULL;

    nenv[i] = strdup(new_var);
    if (!nenv[i]) {
        free_env(nenv);
        return NULL;
    }
    nenv[i + 1] = NULL;
    return nenv;
}


void unset(t_mini *mini, const char *var_name)
{
    if (!mini->envp->env || !var_name || strlen(var_name) == 0) {
        fprintf(stderr, "unset: invalid variable name\n");
        return ;
    }

    int i = 0, j = 0, found = 0;
    int name_len = strlen(var_name);

    // Conta il numero di variabili in env
    while (mini->envp->env[i])
        i++;

    // Alloca un nuovo array per l'ambiente aggiornato
    char **new_env = malloc(sizeof(char *) * i);
    if (!new_env)
            return ;

    i = 0;
    while (mini->envp->env[i]) {
        if (strncmp(mini->envp->env[i], var_name, name_len) == 0 && mini->envp->env[i][name_len] == '=') {
            free(mini->envp->env[i]);
            found = 1;
        } else {
            new_env[j++] = mini->envp->env[i];
        }
        i++;
    }
    new_env[j] = NULL;

    if (!found) {
        fprintf(stderr, "unset: variable '%s' not found\n", var_name);
    }

    free(mini->envp->env);  // Libera l'array originale
    mini->envp->env = new_env;
}
