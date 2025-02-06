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
	else if(b == 2)
	 	ft_cd(token, mini);
	else if(b == 3)
	{
		tmp = ft_pwd(mini->envp->env) ;
		printf("%s\n", tmp);
	}
	else if(b == 4)
	 	 printmatrix(mini->envp->env);
	else if (b == 5)
    {
       export(mini->envp->env, token->next->value);
    }
 	if(b == 6)
 	    unset(mini, token->next->value);
}
