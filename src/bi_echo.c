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

    printf("is: %d/n", g_whatsup);  // Print the value of g_whatsup
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

        // Check if the token is "?" and needs substitution
        if (ft_strcmp(temp->value, "?") == 0)
        {
            printf("%d", g_whatsup);  // Print the value of g_whatsup
        }
        else if (temp->value[0] == '?' && temp->value[1] != '\0')  // Handle cases like "?yeah"
        {
            printf("%d", g_whatsup);  // Print the value of g_whatsup
            printf("%s", &temp->value[1]);  // Print the rest of the string after "?"
        }
        else
        {
            printf("%s", temp->value);  // Normal printing of the token
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

        // Handle substitution for "?" token
        if (ft_strcmp(temp->value, "?") == 0)
        {
            printf("%d", g_whatsup);
        }
        else
        {
            // Handle cases where the value starts with "?" and is followed by other characters
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

    // Print the newline unless the -n flag is present
    if (!flag_n)
        printf("\n");
    ft_exit(0, 0);
}
