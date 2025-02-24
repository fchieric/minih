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
		dup2(state->pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO);
	if (cmd_index < state->cmd_count - 1)
		dup2(state->pipe_fds[cmd_index * 2 + 1], STDOUT_FILENO);
	close_pipes(state);
}

static void	handle_child_process(t_command *cmd, t_mini *mini, t_pipe_state *state,
	int cmd_index)
{
	setup_child_signals();
	setup_child_pipes(state, cmd_index);
	if (cmd->infile)
		redirect_input(cmd->infile);
	if (cmd->outfile)
		redirect_output(cmd->outfile, 0);
	if (cmd->append)
		redirect_output(cmd->append, 1);
	if (cmd->heredoc)
		setup_heredoc(cmd);
	execute_single_command(cmd, mini, STDIN_FILENO, STDOUT_FILENO);
	exit(mini->envp->exit_status);
}

static void	wait_for_children(t_pipe_state *state, t_mini *mini)
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
	g_whatsup = 0;
}

void	execute_commands_with_pipes(t_command *cmd, t_mini *mini)
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
			break ;
		else if (pid == 0)
			handle_child_process(cmd, mini, &state, i);
		else
			state.child_pids[i] = pid;
		cmd = cmd->next;
		i++;
	}
	close_pipes(&state);
	wait_for_children(&state, mini);
	free(state.pipe_fds);
	free(state.child_pids);
}
