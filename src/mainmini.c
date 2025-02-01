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

int main(int ac, char **av, char **env)
{
	t_mini mini;
	(void)ac;
	(void)av;

	inizializer(&mini, env);		//INIZIALIZZA MINI, COPIA ENV E INIZIALIZZA SHELL
	printmatrix(mini.envp->env);	//STAMPA ENV

}
