/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:17:19 by fabi              #+#    #+#             */
/*   Updated: 2025/02/16 16:31:40 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipe_fds(int *pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count * 2)
	{
		close(pipe_fds[i]);
		i++;
	}
}

static int	setup_pipes(t_pipe_state *state)
{
	int	i;

	state->pipe_fds = malloc(sizeof(int) * (state->pipe_count * 2));
	if (!state->pipe_fds)
		return (0);
	i = 0;
	while (i < state->pipe_count)
	{
		if (pipe(state->pipe_fds + (i * 2)) == -1)
		{
			free(state->pipe_fds);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	setup_command_pipes(t_pipe_state *state, int cmd_index)
{
	if (cmd_index > 0)
		dup2(state->pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO);
	if (cmd_index < state->cmd_count - 1)
		dup2(state->pipe_fds[cmd_index * 2 + 1], STDOUT_FILENO);
	close_pipe_fds(state->pipe_fds, state->pipe_count);
}

static int	handle_redirections(t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (0);
	if (cmd->infile && !setup_input_redirection(cmd->infile))
		return (restore_fds(saved_stdin, saved_stdout));
	if (cmd->heredoc && !setup_heredoc(cmd))
		return (restore_fds(saved_stdin, saved_stdout));
	if (cmd->outfile && !setup_output_redirection(cmd->outfile, 0))
		return (restore_fds(saved_stdin, saved_stdout));
	if (cmd->append && !setup_output_redirection(cmd->append, 1))
		return (restore_fds(saved_stdin, saved_stdout));
	return (1);
}

void	execute_external(t_command *cmd, t_mini *mini)
{
	char	*path;

	path = find_command_path(cmd->name, mini->envp->env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->name, 2);
		exit(127);
	}
	execve(path, cmd->args, mini->envp->env);
	free(path);
	exit(127);
}

void	execute_builtin(t_command *cmd, t_mini *mini)
{
	int	builtin_index;

	builtin_index = builtin(cmd->name);
	if (builtin_index != -1)
	{
		// Esegui il builtin corrispondente
		// Da implementare in base alla tua struttura dei builtin
		mini->envp->exit_status = 0;  // Aggiorna lo status in base al risultato
	}
	else
		mini->envp->exit_status = 1;
}

static void	execute_child_process(t_command *cmd, t_mini *mini,
	t_pipe_state *state, int cmd_index)
{
	setup_child_signals();
	setup_command_pipes(state, cmd_index);
	if (!handle_redirections(cmd))
		exit(1);
	if (cmd->type == CMD_BUILTIN)
		execute_builtin(cmd, mini);
	else
		execute_external(cmd, mini);
	exit(mini->envp->exit_status);
}

void	initialize_pipe_state(t_pipe_state *state, int cmd_count)
{
	state->cmd_count = cmd_count;
	state->pipe_count = cmd_count - 1;
	state->pipe_fds = NULL;
	state->child_pids = malloc(sizeof(pid_t) * cmd_count);
	state->original_stdin = dup(STDIN_FILENO);
	state->original_stdout = dup(STDOUT_FILENO);
}

int	cleanup_pipeline(t_pipe_state *state)
{
	if (state->pipe_fds)
		free(state->pipe_fds);
	if (state->child_pids)
		free(state->child_pids);
	if (state->original_stdin != -1)
		close(state->original_stdin);
	if (state->original_stdout != -1)
		close(state->original_stdout);
	return (0);
}

void	wait_for_children(t_pipe_state *state, t_mini *mini)
{
	int	i;
	int	status;

	i = 0;
	while (i < state->cmd_count)
	{
		waitpid(state->child_pids[i], &status, 0);
		if (WIFEXITED(status))
			mini->envp->exit_status = WEXITSTATUS(status);
		i++;
	}
}

int	count_commands(t_command *cmd_list)
{
	int			count;
	t_command	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	execute_pipeline(t_command *cmd_list, t_mini *mini)
{
	t_pipe_state	state;
	t_command		*current;
	int				cmd_index;

	initialize_pipe_state(&state, count_commands(cmd_list));
	if (!setup_pipes(&state))
		return (0);
	current = cmd_list;
	cmd_index = 0;
	while (current)
	{
		state.child_pids[cmd_index] = fork();
		if (state.child_pids[cmd_index] == -1)
			return (cleanup_pipeline(&state));
		else if (state.child_pids[cmd_index] == 0)
			execute_child_process(current, mini, &state, cmd_index);
		current = current->next;
		cmd_index++;
	}
	close_pipe_fds(state.pipe_fds, state.pipe_count);
	wait_for_children(&state, mini);
	cleanup_pipeline(&state);
	return (1);
}