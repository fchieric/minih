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

	temp = token->next;
	while (temp)
	{
		printf("%s", temp->value);
		if (temp->next)
			printf(" ");
		temp = temp->next;
	}
	printf("\n");
}

void	ft_echon(t_token *token)
{
	t_token	*temp;

	temp = token->next;
	while (temp)
	{
		printf("%s", temp->value);
		if (temp->next)
			printf(" ");
		temp = temp->next;
	}
}