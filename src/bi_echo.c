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

	debug_fd("In ft_echo");
	temp = token->next;
	first = 1;
	
	if (!temp)
	{
		write(STDOUT_FILENO, "\n", 1);
		return ;
	}
	
	while (temp)
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

	debug_fd("In ft_echon");
	temp = token->next;
	if (!temp) // Skip il flag -n
		return ;
	temp = temp->next; // Passa al primo argomento dopo -n
	first = 1;
	
	while (temp)
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, temp->value, ft_strlen(temp->value));
		first = 0;
		temp = temp->next;
	}
	// Non stampa la nuova riga
}
