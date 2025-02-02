/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenhandler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:26:20 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/02 18:26:20 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void process_tokens(t_token **token)
{
    t_token *current;

    if (!token || !(*token))
        return;

    current = *token;
    while (current)
    {
        if (current->type == TOKEN_WORD)
            handleword(current);
        else if (current->type == TOKEN_PIPE)
            handlepipe(current);
        else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT ||
                 current->type == TOKEN_HEREDOC || current->type == TOKEN_APPEND)
            handleredirection(current);
        else if (current->type == TOKEN_VAR)
            handlevariable(current);
        else if (current->type == TOKEN_TEXT)
            handletext(current);
        else
            printf("Unknown token: %s\n", current->value);

        current = current->next;
    }
}


// handleword(t_token *token)

// Handles regular words (commands, arguments).
// Example: If the input is "echo", it may store or execute it.
// handlepipe(t_token *token)

// Handles the | (pipe) token.
// This usually connects the output of one command to the input of another.
// handleredirection(t_token *token)

// Handles <, >, >>, << (input/output redirections).
// It should open files, duplicate file descriptors, or manage redirection logic.
// handlevariable(t_token *token)

// Handles shell variables (e.g., $HOME, $USER).
// Likely expands them to their actual values.
// handletext(t_token *token)

// Handles quoted strings ("hello" or 'hello').
// Ensures proper parsing and handling of special characters.