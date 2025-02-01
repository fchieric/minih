/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:39:08 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 16:39:08 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

extern int g_whatsup; //variabile globale che aggiorna lo status della shell e segnala se è in esecuzione o meno

typedef struct s_env
{
	char **env;
	char **exportenv; // export is a copy of env
}t_env;

typedef struct s_mini
{
	t_env *envp;

}t_mini;

void	*safe_malloc(size_t size);
void inizializer(t_mini *mini, int ac, char **av);
void printmatrix (char **matrix);
void copyenv(char **av);

#endif