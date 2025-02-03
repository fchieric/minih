/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlebuiltin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:47:49 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/02 18:47:49 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handleword(t_token *token, t_mini *mini);
int		builtin(char *line);
void builtinexe(t_token *token, t_mini *mini);

int handleword(t_token *token, t_mini *mini)
{
	int b;

	b = builtin(token->value);
	if (b != -1)
		builtinexe(token, mini);
	return (0);
}
int		builtin(char *line)
{
	if (ft_strcmp(line, "echo") == 0)
		return (1);
	if (ft_strcmp(line, "cd") == 0)
		return (2);
	if (ft_strcmp(line, "pwd") == 0)
		return (3);
	if (ft_strcmp(line, "env") == 0)
		return (4);
	if (ft_strcmp(line, "export") == 0)
		return (5);
	if (ft_strcmp(line, "unset") == 0)
		return (6);
	return (-1);
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
void ft_echo(t_token *token)
{
    t_token *temp = token->next;  // Start from the token after the command

    while (temp)  // Iterate through the rest of the tokens (arguments)
    {
        printf("%s", temp->value);  // Print each argument
        if (temp->next)  // If not the last argument, print space
            printf(" ");
        temp = temp->next;
    }
    printf("\n");
}

void ft_echon(t_token *token)
{
    t_token *temp = token->next;

    while (temp)
    {
        printf("%s", temp->value);
        if (temp->next)
            printf(" ");
        temp = temp->next;
    }
}

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

void builtinexe(t_token *token, t_mini *mini)
{
	int b;
	char *tmp;

	b = builtin(token->value);

	 if(b == 1)
	{
		if(token->next)
			tmp = token->next->value;
		if (ft_strcmp(tmp, "-n") == 0)
			ft_echon(token);
		else
		ft_echo(token);
	}
	if(b == 2)
	 	ft_cd(token, mini);
	if(b == 3)
	{
		tmp = ft_pwd(mini->envp->env) ;
		printf("%s\n", tmp);
	}
	if(b == 4)
	 	 printmatrix(mini->envp->env);
	// if(b == 5)
	// 	mini->envp->env = export(mini->envp->env, token->next->value);
	// if(b = 6)
	// 	env = unset_env(env, "-----------------")
	// free_env(env);
}
