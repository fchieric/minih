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
        perror("cd: getcwd failed");
        ft_exit(1, 0);
        return;
    }
    if (token->next)
        path = token->next->value;
    if (!path || strcmp(path, "~") == 0)
    {
        path = ft_getenv(mini->envp->env, "HOME");
        if (!path)
        {
            fprintf(stderr, "cd: HOME not set\n");
            free(old_pwd);
            ft_exit(1, 0);
            return;
        }
    }
    else if (strcmp(path, "-") == 0)
    {
        path = ft_getenv(mini->envp->env, "OLDPWD");
        if (!path)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(old_pwd);
            ft_exit(1, 0);
            return;
        }
        printf("%s\n", path);
    }
    if (chdir(path) != 0)
    {
        fprintf(stderr, "cd: %s: ", path);
        perror("");
        ft_exit(1, 0);
        free(old_pwd);
        return;
    }
    ft_setenv(&(mini->envp->env), "OLDPWD", old_pwd);
    free(old_pwd);
    char *new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        ft_putstr_fd("cd: getcwd failed", 2);
        ft_exit(1, 0);
        return;
    }
    ft_setenv(&(mini->envp->env), "PWD", new_pwd);
    ft_exit(0, 0);
    free(new_pwd);
}


char *ft_pwd(char **env)
{
    int i = 0;
    ft_exit(0, 0);
    while (env[i] != NULL)
	{
        if (strncmp(env[i], "PWD=", 4) == 0)
            return (env[i] + 4);
        i++;
    }
    ft_putstr_fd("PWD not found in environment.\n", 2);
    ft_exit(1, 0);
	return(NULL);
}