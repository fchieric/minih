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

char **copyenv(char **envp);
void free_env(char **env);

char **copyenv(char **envp)
{
    int count = 0;
    while (envp[count])
        count++;

    char **env = (char **)malloc(sizeof(char *) * (count + 2));
    if (!env)
        return NULL;

    int i = 0;
    int oldpwd_found = 0;

    while (i < count)
    {
        env[i] = strdup(envp[i]);
        if (!env[i])
        {
            while (i > 0)
                free(env[--i]);
            free(env);
            return NULL;
        }
        if (strncmp(env[i], "OLDPWD=", 7) == 0)
            oldpwd_found = 1;
        i++;
    }
    if (!oldpwd_found)
    {
        env[i] = strdup("OLDPWD=");
        if (!env[i])
        {
            while (i > 0)
                free(env[--i]);
            free(env);
            return NULL;
        }
        i++;
    }

    env[i] = NULL;
    return env;
}

void free_env(char **env)
{
    int i = 0;
    if (!env)
        return;

    while (env[i])
        free(env[i++]);

    free(env);
}

char *ft_getenv(char **envp, const char *name)
{
    int i = 0;
    size_t len = strlen(name);

    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return NULL;
}

void ft_setenv(char ***envp, const char *name, const char *value)
{
    int i = 0;
    size_t len = strlen(name);
    char *new_value;
    char **env = *envp;

    new_value = malloc(strlen(name) + strlen(value) + 2);
    if (!new_value)
        return;
    sprintf(new_value, "%s=%s", name, value);

    while (env[i])
    {
        if (strncmp(env[i], name, len) == 0 && env[i][len] == '=')
        {
            free(env[i]);
            env[i] = new_value;
            return;
        }
        i++;
    }
    char **new_env = realloc(env, sizeof(char *) * (i + 2));
    if (!new_env)
    {
        free(new_value);
        return;
    }
    new_env[i] = new_value;
    new_env[i + 1] = NULL;
    *envp = new_env;
}