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

char	**copyenv(char **envp)
{
	int		count;
	char	**env;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (i > 0)
				free(env[--i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[count] = NULL;
	return (env);
}

void free_env(char **env)
{
	int i = 0;

	if (!env)
		return;

	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

// Function to get an environment variable
char *ft_getenv(char **envp, const char *name)
{
    int i = 0;
    size_t len = strlen(name);

    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return envp[i] + len + 1;
        i++;
    }
    return NULL;
}

// Function to update the environment variable
void ft_setenv(char **envp, const char *name, const char *value)
{
    int i = 0;
    size_t len = strlen(name);
    char *new_value;

    new_value = malloc(strlen(name) + strlen(value) + 2);
    if (!new_value)
        return;
    sprintf(new_value, "%s=%s", name, value);

    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
        {
            free(envp[i]);
            envp[i] = new_value;
            return;
        }
        i++;
    }
}