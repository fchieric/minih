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

void	init_shell(t_mini *mini, char **env)
{
    mini->envp = safe_malloc(sizeof(t_env));
    mini->envp->env = copyenv(env);
    mini->envp->exportenv = copyenv(env);
    mini->envp->exit_status = 0;
}