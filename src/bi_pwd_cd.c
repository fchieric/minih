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
        g_whatsup = 1;
        return;
    }

    // Se viene passato un argomento, lo usiamo come percorso
    if (token->next)
        path = token->next->value;

    // Se nessun argomento o "~", andiamo nella HOME
    if (!path || strcmp(path, "~") == 0)
    {
        path = ft_getenv(mini->envp->env, "HOME");
        if (!path)
        {
            fprintf(stderr, "cd: HOME not set\n");
            free(old_pwd);
            g_whatsup = 1;
            return;
        }
    }
    // Se "-", torniamo nella directory precedente
    else if (strcmp(path, "-") == 0)
    {
        path = ft_getenv(mini->envp->env, "OLDPWD");
        if (!path)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            g_whatsup = 1;
            free(old_pwd);
            return;
        }
        printf("%s\n", path); // Stampa la directory quando si usa "cd -"
    }

    // Cambiamo la directory
    if (chdir(path) != 0)
    {
        fprintf(stderr, "cd: %s: ", path);
        perror("");
        free(old_pwd);
        g_whatsup = 1;
        return;
    }

    // Aggiorniamo OLDPWD e PWD
    ft_setenv(&(mini->envp->env), "OLDPWD", old_pwd);
    free(old_pwd);

    char *new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("cd: getcwd failed");
        g_whatsup = 1;
        return;
    }

    ft_setenv(&(mini->envp->env), "PWD", new_pwd);
    g_whatsup = 0;
    free(new_pwd);
}


char *ft_pwd(char **env)
{
    int i = 0;
    g_whatsup = 0;
    while (env[i] != NULL)
	{
        if (strncmp(env[i], "PWD=", 4) == 0)
            return (env[i] + 4);
        i++;
    }
    g_whatsup = 1;
    printf("PWD not found in environment.\n");
	return(NULL);
}