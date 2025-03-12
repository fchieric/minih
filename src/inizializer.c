/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inizializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:39:20 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 18:39:20 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_mini *mini, char **env);

void init_shell(t_mini *mini, char **env)
{
    char *shlvl_str;
    int shlvl;
    char new_shlvl[12];

    mini->envp = safe_malloc(sizeof(t_env));
    mini->envp->env = copyenv(env);
    mini->envp->exportenv = copyenv(env);
    g_whatsup = 0;
    shlvl_str = ft_getenv(mini->envp->env, "SHLVL");
    if (shlvl_str)
    shlvl = atoi(shlvl_str);
    else
    shlvl = 0;
    shlvl++;
    sprintf(new_shlvl, "%d", shlvl);
    ft_setenv(&(mini->envp->env), "SHLVL", new_shlvl);
}
