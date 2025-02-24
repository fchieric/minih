/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:34:42 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/24 23:41:14 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd, char **envp)
{
	char	**all_paths;
	char	*path;
	char	*path_cmd;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
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
		if (access(path_cmd, F_OK | X_OK) == 0)
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

static void	handle_builtin_redirects(int input_fd, int output_fd, int *saved)
{
	debug_fd("Before handle_builtin_redirects");
	if (input_fd != STDIN_FILENO || output_fd != STDOUT_FILENO)
	{
		saved[0] = dup(STDIN_FILENO);
		saved[1] = dup(STDOUT_FILENO);
		if (input_fd != STDIN_FILENO)
			dup2(input_fd, STDIN_FILENO);
		if (output_fd != STDOUT_FILENO)
			dup2(output_fd, STDOUT_FILENO);
	}
	debug_fd("After handle_builtin_redirects");
}

static void	restore_fds(int *saved_fds)
{
	debug_fd("Before restore_fds");
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
	debug_fd("After restore_fds");
}

static t_token	*create_token_from_cmd(t_command *cmd)
{
	t_token	*token;
	t_token	*last;
	int		i;

	token = safe_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = TOKEN_WORD;
	token->value = ft_strdup(cmd->name);
	token->next = NULL;
	last = token;
	i = 1;
	while (cmd->args[i])
	{
		last->next = safe_malloc(sizeof(t_token));
		if (!last->next)
			break ;
		last = last->next;
		last->type = TOKEN_WORD;
		last->value = ft_strdup(cmd->args[i]);
		last->next = NULL;
		i++;
	}
	return (token);
}

void	execute_single_command(t_command *cmd, t_mini *mini,
	int input_fd, int output_fd)
{
	int		saved_fds[2];
	t_token	*token;
	char	*path;

	debug_fd("Before execute_single_command");
	saved_fds[0] = -1;
	saved_fds[1] = -1;
	if (cmd->type == CMD_BUILTIN)
	{
		handle_builtin_redirects(input_fd, output_fd, saved_fds);
		token = create_token_from_cmd(cmd);
		if (token)
		{
			debug_fd("Before builtin in single command");
			handleword(token, mini);
			debug_fd("After builtin in single command");
			free_tokens(token);
		}
		restore_fds(saved_fds);
		return ;
	}
	path = find_command_path(cmd->name, mini->envp->env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->name, 2);
		mini->envp->exit_status = 127;
		return ;
	}
	if (input_fd != STDIN_FILENO)
		dup2(input_fd, STDIN_FILENO);
	if (output_fd != STDOUT_FILENO)
		dup2(output_fd, STDOUT_FILENO);
	execve(path, cmd->args, mini->envp->env);
	free(path);
	exit(127);
	debug_fd("After execute_single_command");
}
