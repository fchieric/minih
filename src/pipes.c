/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:14:38 by marvin            #+#    #+#             */
/*   Updated: 2025/02/24 16:14:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_commands(t_command *cmd)
{
	int			count;
	t_command	*current;

	count = 0;
	current = cmd;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static int	setup_pipes(t_pipe_state *state)
{
	int	i;

	state->pipe_count = state->cmd_count - 1;
	if (state->pipe_count <= 0)
		return (0);
	state->pipe_fds = malloc(sizeof(int) * 2 * state->pipe_count);
	if (!state->pipe_fds)
		return (-1);
	i = 0;
	while (i < state->pipe_count)
	{
		if (pipe(state->pipe_fds + i * 2) < 0)
		{
			perror("pipe");
			free(state->pipe_fds);
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	close_pipes(t_pipe_state *state)
{
	int	i;

	i = 0;
	while (i < state->pipe_count * 2)
	{
		close(state->pipe_fds[i]);
		i++;
	}
}

static void	setup_child_pipes(t_pipe_state *state, int cmd_index)
{
	if (cmd_index > 0)
	{
		dup2(state->pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO);
	}
	if (cmd_index < state->cmd_count - 1)
	{
		dup2(state->pipe_fds[cmd_index * 2 + 1], STDOUT_FILENO);
	}
	close_pipes(state);
}

static void	execute_commands_with_pipes(t_command *cmd, t_mini *mini)
{
	t_pipe_state	state;
	int				i;
	pid_t			pid;

	state.cmd_count = count_commands(cmd);
	state.child_pids = malloc(sizeof(pid_t) * state.cmd_count);
	if (setup_pipes(&state) < 0 || !state.child_pids)
		return ;
	g_whatsup = 1;
	i = 0;
	while (cmd && i < state.cmd_count)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			break ;
		}
		else if (pid == 0)
		{
			setup_child_signals();
			setup_child_pipes(&state, i);
			execute_single_command(cmd, mini, STDIN_FILENO, STDOUT_FILENO);
			exit(mini->envp->exit_status);
		}
		else
		{
			state.child_pids[i] = pid;
		}
		cmd = cmd->next;
		i++;
	}
	close_pipes(&state);
	i = 0;
	while (i < state.cmd_count)
	{
		waitpid(state.child_pids[i], &mini->envp->exit_status, 0);
		if (WIFEXITED(mini->envp->exit_status))
			mini->envp->exit_status = WEXITSTATUS(mini->envp->exit_status);
		i++;
	}
	g_whatsup = 0;
	free(state.pipe_fds);
	free(state.child_pids);
}

void	execute_commands(t_command *cmd, t_mini *mini)
{
	if (!cmd)
		return ;
	
	if (!cmd->next && cmd->type == CMD_BUILTIN)
	{
		// For single builtin commands, no need to fork
		if (cmd->infile)
			setup_redirections(cmd);
		handle_builtin(cmd, mini);
	}
	else if (!cmd->next)
	{
		// For single external commands
		execute_external_command(cmd, mini);
	}
	else
	{
		// For piped commands
		execute_commands_with_pipes(cmd, mini);
	}
}