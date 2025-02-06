/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:48:59 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/06 16:48:59 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_target_var(const char *env_var, const char *var_name, int name_len)
{
	return (strncmp(env_var, var_name, name_len) == 0 && env_var[name_len] == '=');
}

static void	copy_env_except_var(char **new_env, char **old_env, const char *var_name, int name_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (old_env[i])
	{
		if (!is_target_var(old_env[i], var_name, name_len))
			new_env[j++] = old_env[i];
		else
			free(old_env[i]);
		i++;
	}
	new_env[j] = NULL;
}

void	unset(t_mini *mini, const char *var_name)
{
	int		env_size;
	int		name_len;
	char	**new_env;

	if (!mini->envp->env || !var_name || strlen(var_name) == 0)
	{
		fprintf(stderr, "unset: invalid variable name\n");
		return ;
	}
	name_len = strlen(var_name);
	env_size = 0;
	while (mini->envp->env[env_size])
		env_size++;
	new_env = malloc(sizeof(char *) * env_size);
	if (!new_env)
		return ;
	copy_env_except_var(new_env, mini->envp->env, var_name, name_len);
	free(mini->envp->env);
	mini->envp->env = new_env;
}

// void unset(t_mini *mini, const char *var_name)
// {
// 	int i;
// 	int j;
// 	int found;
// 	int name_len;
// 	char **new_env;

// 	i = 0;
// 	j = 0;
// 	found = 0;
// 	if (!mini->envp->env || !var_name || strlen(var_name) == 0)
// 	{
// 		fprintf(stderr, "unset: invalid variable name\n");
// 		return ;
// 	}
// 	name_len = strlen(var_name);
// 	while (mini->envp->env[i])
// 		i++;
// 	new_env = malloc(sizeof(char *) * i);
// 	if (!new_env)
// 			return ;
// 	i = 0;
// 	while (mini->envp->env[i])
// 	{
// 		if (strncmp(mini->envp->env[i], var_name, name_len) == 0 && mini->envp->env[i][name_len] == '=')
// 		{
// 			free(mini->envp->env[i]);
// 			found = 1;
// 		}
// 		else
// 			new_env[j++] = mini->envp->env[i];
// 		i++;
// 	}
// 	new_env[j] = NULL;
// 	if (!found)
// 		fprintf(stderr, "unset: variable '%s' not found\n", var_name);
// 	free(mini->envp->env);
// 	mini->envp->env = new_env;
// }
