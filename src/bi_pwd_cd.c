/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:47:29 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/06 16:47:29 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_pwd(char **env);
void ft_cd(t_token *token, t_mini *mini);

void ft_cd(t_token *token, t_mini *mini)
{
    char *path = NULL;
    char *old_pwd = getcwd(NULL, 0);

    if (!old_pwd)
    {
        perror("cd");
        return;
    }
    if (token->next)
        path = token->next->value;

    if (!path || strcmp(path, "~") == 0)
        path = ft_getenv(mini->envp->env, "HOME");
    else if (strcmp(path, "-") == 0)
        path = ft_getenv(mini->envp->env, "OLDPWD");
    if (!path)
    {
        fprintf(stderr, "cd: No such file or directory\n");
        free(old_pwd);
        return;
    }
    if (chdir(path) != 0)
        perror("cd");
    else
    {
        ft_setenv(mini->envp->env, "OLDPWD", old_pwd);
        char *new_pwd = getcwd(NULL, 0);
        ft_setenv(mini->envp->env, "PWD", new_pwd);
        free(new_pwd);
    }
    free(old_pwd);
}

char *ft_pwd(char **env)
{
    int i = 0;
    while (env[i] != NULL)
	{
        if (strncmp(env[i], "PWD=", 4) == 0)
            return (env[i] + 4);
        i++;
    }
    printf("PWD not found in environment.\n");
	return(NULL);
}