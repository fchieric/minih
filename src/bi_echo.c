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

void ft_echo(t_token *token)
{
    t_token *temp;
    int first;

    temp = token->next;
    first = 1;

    if (!temp)
    {
        printf("\n");
        return;
    }
    while (temp && temp->type == TOKEN_WORD)
    {
        if (!first)
            printf(" ");
        if (ft_strcmp(temp->value, "?") == 0)
        {
            printf("%d", g_whatsup);
        }
        else if (temp->value[0] == '?' && temp->value[1] != '\0')
        {
            printf("%d", g_whatsup);
            printf("%s", &temp->value[1]);
        }
        else
        {
            printf("%s", temp->value);
        }

        first = 0;
        temp = temp->next;
    }
    printf("\n");
    ft_exit(0, 0);
}

void ft_echon(t_token *token)
{
    t_token *temp;
    int first;
    int flag_n = 0;

    temp = token->next;
    if (!temp)
        return ;
    while (temp && temp->value[0] == '-' && temp->value[1] == 'n')
    {
        int i = 1;
        while (temp->value[i] == 'n')
            i++;
        if (temp->value[i] != '\0')
            break;
        temp = temp->next;
        flag_n = 1;
    }

    first = 1;
    while (temp && temp->type == TOKEN_WORD)
    {
        if (!first)
            printf(" ");

        if (ft_strcmp(temp->value, "?") == 0)
        {
            printf("%d", g_whatsup);
        }
        else
        {
            if (temp->value[0] == '?' && temp->value[1] != '\0')
            {
                printf("%d", g_whatsup);
                printf("%s", &temp->value[1]);
            }
            else
            {
                printf("%s", temp->value);
            }
        }

        first = 0;
        temp = temp->next;
    }
    if (!flag_n)
        printf("\n");
    ft_exit(0, 0);
}
