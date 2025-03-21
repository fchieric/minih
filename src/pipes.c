/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:14:38 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/24 16:14:38 by fmartusc         ###   ########.fr       */
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

static void	handle_child_process(t_command *cmd, t_mini *mini,
	t_pipe_state *state, int cmd_index)
{
	setup_child_signals();
	setup_child_pipes(state, cmd_index);

	// Configura le redirezioni
	if (cmd->infile)
		redirect_input(cmd->infile);
	if (cmd->outfile)
		redirect_output(cmd->outfile, 0);
	if (cmd->append)
		redirect_output(cmd->append, 1);
	if (cmd->heredoc)
		setup_heredoc(cmd);

	if (cmd->type == CMD_BUILTIN)
	{
		t_token *token = safe_malloc(sizeof(t_token));
		if (!token)
			exit(1);
		token->type = TOKEN_WORD;
		token->value = ft_strdup(cmd->name);
		token->next = NULL;

		// Aggiungi tutti gli argomenti come token
		t_token *last = token;
		int i = 1;
		while (cmd->args[i])
		{
			last->next = safe_malloc(sizeof(t_token));
			if (!last->next)
			{
				g_whatsup = 126;
				exit(126);
			}
			last = last->next;
			last->type = TOKEN_WORD;
			last->value = ft_strdup(cmd->args[i]);
			last->next = NULL;
			i++;
		}
		handleword(token, mini);

		// Libera i token
		free_tokens(token);
		exit(g_whatsup);
	}
	else
	{
		// Per i comandi esterni, usa execve
		char *path = find_command_path(cmd->name, mini->envp->env);
		// Modifica questa parte
		if (!path)
		{
	    	ft_putstr_fd("minishell: command not found: ", 2);
	    	ft_putendl_fd(cmd->name, 2);
    		g_whatsup = 127; // Questa impostazione non ha effetto nel processo padre
    		exit(127);
		}
		execve(path, cmd->args, mini->envp->env);
		free(path);
		g_whatsup = 127; // Anche questa impostazione non ha effetto
		exit(127);
	}
}

static void	wait_for_children(t_pipe_state *state)
{
	int	i;
	int	status;

	i = 0;
	while (i < state->cmd_count)
	{
		waitpid(state->child_pids[i], &status, 0);
		if (WIFEXITED(status))
			g_whatsup = WEXITSTATUS(status);
		i++;
	}
	g_whatsup = 0;
}

void	execute_commands_with_pipes(t_command *cmd, t_mini *mini)
{
	t_pipe_state	state;
	int				i;
	t_command		*current;

	state.cmd_count = count_commands(cmd);
	state.child_pids = malloc(sizeof(pid_t) * state.cmd_count);
	if (setup_pipes(&state) < 0 || !state.child_pids)
		return ;
	g_whatsup = 1;
	i = 0;
	current = cmd;
	while (current && i < state.cmd_count)
	{
		state.child_pids[i] = fork();
		if (state.child_pids[i] < 0)
			break ;
		else if (state.child_pids[i] == 0)
			handle_child_process(current, mini, &state, i);
		current = current->next;
		i++;
	}
	close_pipes(&state);
	wait_for_children(&state);
	free(state.pipe_fds);
	free(state.child_pids);
}
