/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:41:32 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/06 16:41:32 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**export(char **env, const char *new_var);
static void	printdeclare(char **env);
static int	update_existing_var(char **env, const char *new_var, int name_len);

static void	printdeclare(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
}

static int	update_existing_var(char **env, const char *new_var, int name_len)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], new_var, name_len) == 0 && env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = strdup(new_var);
			if (!env[i])
				return (0);
			return (1);
		}
		i++;
	}
	return (0);
}

char	**export(char **env, const char *new_var)
{
	char	**nenv;
	int		name_len;
	int		i;

	if (!new_var || strchr(new_var, '=') == NULL)
	{
		g_whatsup = 1;
		return (printdeclare(env), env);
	}
	name_len = strchr(new_var, '=') - new_var;
	if (update_existing_var(env, new_var, name_len))
		return (env);
	i = 0;
	while (env[i])
		i++;
	nenv = (char **)realloc(env, sizeof(char *) * (i + 2));
	if (!nenv)
	{
		g_whatsup = 1;
		return (NULL);
	}
	nenv[i] = strdup(new_var);
	if (!nenv[i])
	{
		free_env(nenv);
		g_whatsup = 1;
		return (NULL);
	}
	nenv[i + 1] = NULL;
	g_whatsup = 0;
	return (nenv);
}