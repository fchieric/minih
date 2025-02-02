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

void builtinexe(t_cmd *cmd_list) //passare qullo chee edo passa all'executor
{
	int b;
	//char **env;

	b = builtin(cmd_list->word); //ciclare word
	//**env = copyenv(envp)

	// if(b = 1)
	// 	fecho
	// if(b = 2)
	// 	fcd
	if(b == 3)
		ft_pwd() ;
	ft_printf("daje\n");
	// if(b = 4)
	// 	 print_env(env);
	// if(b = 5)
	// 	env = export(env, "----------------")
	// if(b = 6)
	// 	env = unset_env(env, "-----------------")
	// free_env(env);
}
