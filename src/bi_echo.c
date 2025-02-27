/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:53:32 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/06 16:53:32 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_token *token);
void	ft_echon(t_token *token);

void	ft_echo(t_token *token)
{
	t_token	*temp;
	int		first;

	temp = token->next;
	first = 1;

	if (!temp)
	{
		write(STDOUT_FILENO, "\n", 1);
		return ;
	}
	while (temp && temp->type == TOKEN_WORD)
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, temp->value, ft_strlen(temp->value));
		first = 0;
		temp = temp->next;
	}
	write(STDOUT_FILENO, "\n", 1);
}

void	ft_echon(t_token *token)
{
	t_token	*temp;
	int		first;

	temp = token->next;

	if (!temp)
		return ;
	// Salta tutti i "-n" validi
	while (temp && temp->value[0] == '-' && temp->value[1] == 'n')
	{
		int i = 1;
		while (temp->value[i] == 'n') // Controlla che sia tutto "n"
			i++;
		if (temp->value[i] != '\0') // Se c'è un altro carattere oltre "n", fermati
			break;
		temp = temp->next;
	}

	first = 1;
	while (temp)
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, temp->value, strlen(temp->value));
		first = 0;
		temp = temp->next;
	}
}