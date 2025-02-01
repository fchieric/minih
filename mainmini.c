/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainmini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:50:04 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 16:50:04 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_whatsup = 0;

void inizializer(t_mini *mini, int ac, char **av)
{
	mini->envp = safe_malloc(sizeof(t_env));
	mini->envp->env = copyenv(av);
	mini->envp->exportenv = copyenv(av);
}

void printmatrix (char **matrix)
{
	int i = 0;
	while (matrix[i])
	{
		printf("%s\n", matrix[i]);
		i++;
	}
}

int main(int ac, char **av)
{
	t_mini mini;

	inizializer(&mini, ac, av);		//INIZIALIZZA MINI, COPIA ENV E INIZIALIZZA SHELL
	printmatrix(mini.envp->env);	//STAMPA ENV

}