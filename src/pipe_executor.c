/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:34:42 by fabi              #+#    #+#             */
/*   Updated: 2025/02/24 16:17:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_command_path(char *cmd, char **envp)
{
    char    **all_paths;
    char    *path;
    int     i;
    char    *path_cmd;

    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));

    i = 0;
    while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    if (!envp[i])
        return (NULL);
        
    all_paths = ft_split(envp[i] + 5, ':');
    i = 0;
    while (all_paths[i])
    {
        path = ft_strjoin(all_paths[i], "/");
        path_cmd = ft_strjoin(path, cmd);
        free(path);
        if (access(path_cmd, F_OK) == 0)
        {
            free_matrix(all_paths);
            return (path_cmd);
        }
        free(path_cmd);
        i++;
    }
    free_matrix(all_paths);
    return (NULL);
}

static void    handle_builtin_redirects(int input_fd, int output_fd, 
    int *saved_fds)
{
    if (input_fd != STDIN_FILENO || output_fd != STDOUT_FILENO)
    {
        saved_fds[0] = dup(STDIN_FILENO);
        saved_fds[1] = dup(STDOUT_FILENO);
        if (input_fd != STDIN_FILENO)
            dup2(input_fd, STDIN_FILENO);
        if (output_fd != STDOUT_FILENO)
            dup2(output_fd, STDOUT_FILENO);
    }
}

static void    restore_fds(int *saved_fds)
{
    if (saved_fds[0] != -1)
    {
        dup2(saved_fds[0], STDIN_FILENO);
        close(saved_fds[0]);
    }
    if (saved_fds[1] != -1)
    {
        dup2(saved_fds[1], STDOUT_FILENO);
        close(saved_fds[1]);
    }
}

void execute_single_command(t_command *cmd, t_mini *mini, 
    int input_fd, int output_fd)
{
    char    *path;
    int     saved_fds[2];
    t_token *token;

    saved_fds[0] = -1;
    saved_fds[1] = -1;
    if (cmd->type == CMD_BUILTIN)
    {
        handle_builtin_redirects(input_fd, output_fd, saved_fds);
        
        // Create a token for handleword using cmd's name
        token = safe_malloc(sizeof(t_token));
        if (!token)
            return;
        
        token->type = TOKEN_WORD;
        token->value = ft_strdup(cmd->name);
        
        // Create a linked list of tokens for arguments if needed
        t_token *last = token;
        for (int i = 1; cmd->args[i] != NULL; i++)
        {
            t_token *arg_token = safe_malloc(sizeof(t_token));
            if (!arg_token)
                break;
            
            arg_token->type = TOKEN_WORD;
            arg_token->value = ft_strdup(cmd->args[i]);
            arg_token->next = NULL;
            
            last->next = arg_token;
            last = arg_token;
        }
        
        // Call handleword with the token
        handleword(token, mini);
        
        // Free the tokens
        free_tokens(token);
        
        restore_fds(saved_fds);
        return;
    }
    
    // For external commands, keep the original code
    path = find_command_path(cmd->name, mini->envp->env);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(cmd->name, 2);
        mini->envp->exit_status = 127;
        return;
    }
    
    if (input_fd != STDIN_FILENO)
        dup2(input_fd, STDIN_FILENO);
    if (output_fd != STDOUT_FILENO)
        dup2(output_fd, STDOUT_FILENO);
    
    execve(path, cmd->args, mini->envp->env);
    free(path);
    exit(127);
}
