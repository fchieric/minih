/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:34:42 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 23:48:09 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int count_commands(t_command *cmd)
{
    int count;
    
    count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return (count);
}

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

void execute_single_command(t_command *cmd, t_mini *mini, int input_fd, int output_fd)
{
    char *path;

    if (cmd->type == CMD_BUILTIN)
    {
        // Gestisce builtin con redirezioni
        int saved_stdin = -1;
        int saved_stdout = -1;

        if (input_fd != STDIN_FILENO || output_fd != STDOUT_FILENO || 
            cmd->infile || cmd->outfile || cmd->append || cmd->heredoc)
        {
            saved_stdin = dup(STDIN_FILENO);
            saved_stdout = dup(STDOUT_FILENO);
        }

        // Gestione dell'input
        if (input_fd != STDIN_FILENO)
            dup2(input_fd, STDIN_FILENO);
        if (output_fd != STDOUT_FILENO)
            dup2(output_fd, STDOUT_FILENO);

        // Esegue il builtin
        //handle_builtin(cmd, mini);

        // Ripristina i file descriptors
        if (saved_stdin != -1)
        {
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }
        if (saved_stdout != -1)
        {
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
    }
    else
    {
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
}

static void handle_child_process(t_command *cmd, t_mini *mini, 
                               t_pipe_state *state, int cmd_index)
{
    // Gestione dell'input
    if (cmd_index == 0)  // Primo comando
    {
        if (cmd->infile || cmd->heredoc)
            ; // L'input verrà gestito in execute_single_command
        else if (state->pipe_count > 0)
            dup2(state->pipe_fds[1], STDOUT_FILENO);
    }
    else if (cmd_index == state->cmd_count - 1)  // Ultimo comando
    {
        if (!cmd->outfile && !cmd->append)
            dup2(state->pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO);
    }
    else  // Comandi intermedi
    {
        dup2(state->pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO);
        dup2(state->pipe_fds[cmd_index * 2 + 1], STDOUT_FILENO);
    }

    // Chiude tutti i file descriptor delle pipe nel processo figlio
    for (int i = 0; i < state->pipe_count * 2; i++)
        if (state->pipe_fds[i] > 2)
            close(state->pipe_fds[i]);

    // Esegue il comando con le eventuali redirezioni
    execute_single_command(cmd, mini, STDIN_FILENO, STDOUT_FILENO);
    exit(mini->envp->exit_status);
}
