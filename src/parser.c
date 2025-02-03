/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:27:22 by fabi              #+#    #+#             */
/*   Updated: 2025/02/02 01:01:54 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_builtin(const char *cmd)
{
    static const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    int i;

    i = 0;
    while (builtins[i])
    {
        if (ft_strcmp(cmd, builtins[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}

static t_command *init_command(void)
{
    t_command *cmd;

    cmd = safe_malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->name = NULL;
    cmd->args = NULL;
    cmd->flags = NULL;
    cmd->type = CMD_NONE;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->heredoc = NULL;
    cmd->append = NULL;
    cmd->next = NULL;
    return (cmd);
}

static char **add_to_array(char **arr, char *str)
{
    char **new_arr;
    int len;
    int i;

    len = 0;
    if (arr)
        while (arr[len])
            len++;
    new_arr = safe_malloc(sizeof(char *) * (len + 2));
    if (!new_arr)
        return (NULL);
    i = 0;
    while (i < len)
    {
        new_arr[i] = arr[i];
        i++;
    }
    new_arr[i] = ft_strdup(str);
    new_arr[i + 1] = NULL;
    free(arr);
    return (new_arr);
}

static void process_word_token(t_command *cmd, char *value)
{
    if (!cmd->name)
    {
        cmd->name = ft_strdup(value);
        cmd->type = is_builtin(value) ? CMD_BUILTIN : CMD_EXTERNAL;
        cmd->args = add_to_array(cmd->args, value);
    }
    else if (value[0] == '-')
    {
        cmd->flags = add_to_array(cmd->flags, value);
        cmd->args = add_to_array(cmd->args, value);
    }
    else
        cmd->args = add_to_array(cmd->args, value);
}

t_command *parse_tokens(t_token *tokens)
{
    t_command *cmd;
    t_command *head;
    t_token *curr;

    if (!tokens)
        return (NULL);
    cmd = init_command();
    head = cmd;
    curr =kens;
    while (curr)
    {
        if (curr->type == TOKEN_WORD || curr->type == TOKEN_TEXT || 
            curr->type == TOKEN_VAR)
            process_word_token(cmd, curr->value);
        else if (curr->type == TOKEN_PIPE)
        {
            cmd->next = init_command();
            cmd = cmd->next;
        }
        else if (curr->type == TOKEN_REDIR_IN && curr->next)
            cmd->infile = ft_strdup(curr->next->value);
        else if (curr->type == TOKEN_REDIR_OUT && curr->next)
            cmd->outfile = ft_strdup(curr->next->value);
        else if (curr->type == TOKEN_HEREDOC && curr->next)
            cmd->heredoc = ft_strdup(curr->next->value);
        else if (curr->type == TOKEN_APPEND && curr->next)
            cmd->append = ft_strdup(curr->next->value);
        curr = curr->next;
    }
    return (head);
}
