/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenhandler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:26:20 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/25 16:30:42 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_fd(const char *message)
{
	int	stdin_fd;
	int	stdout_fd;
	int	stderr_fd;

	stdin_fd = fcntl(STDIN_FILENO, F_GETFL);
	stdout_fd = fcntl(STDOUT_FILENO, F_GETFL);
	stderr_fd = fcntl(STDERR_FILENO, F_GETFL);
	fprintf(stderr, "DEBUG %s: stdin=%d, stdout=%d, stderr=%d\n",
		message, stdin_fd, stdout_fd, stderr_fd);
}

void	process_tokens(t_token *tokens, t_mini *mini)
{
	t_command	*cmd;

	if (!tokens)
		return ;
	cmd = parse_tokens(tokens);
	if (!cmd)
		return ;
	execute_commands(cmd, mini);
	free_commands(cmd);
}

void	execute_commands(t_command *cmd, t_mini *mini)
{
	t_token	*tmp_token;
	int		saved_stdout;
	int		saved_stdin;

	if (!cmd)
		return ;
	debug_fd("Before execute_commands");
	saved_stdout = -1;
	saved_stdin = -1;
	if (cmd->outfile || cmd->append || cmd->infile)
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
	}
	if (cmd->infile)
		redirect_input(cmd->infile);
	if (cmd->outfile)
		redirect_output(cmd->outfile, 0);
	if (cmd->append)
		redirect_output(cmd->append, 1);
	if (!cmd->next)
	{
		if (cmd->type == CMD_BUILTIN)
		{
			tmp_token = safe_malloc(sizeof(t_token));
			if (!tmp_token)
				return ;
			tmp_token->type = TOKEN_WORD;
			tmp_token->value = ft_strdup(cmd->args[0]);
			tmp_token->next = NULL;
			debug_fd("Before builtin execution");
			handleword(tmp_token, mini);
			debug_fd("After builtin execution");
			free(tmp_token->value);
			free(tmp_token);
		}
		else if (cmd->type == CMD_EXTERNAL)
			execute_external_command(cmd, mini);
	}
	else
		execute_commands_with_pipes(cmd, mini);
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	debug_fd("After execute_commands");
}

void	setup_redirections(t_command *cmd)
{
	debug_fd("Before setup_redirections");
	if (cmd->infile)
		redirect_input(cmd->infile);
	if (cmd->outfile)
		redirect_output(cmd->outfile, 0);
	if (cmd->append)
		redirect_output(cmd->append, 1);
	if (cmd->heredoc)
		setup_heredoc(cmd);
	debug_fd("After setup_redirections");
}

void	execute_external_command(t_command *cmd, t_mini *mini)
{
	pid_t	pid;
	int		status;

	debug_fd("Before execute_external_command");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		setup_child_signals();
		setup_redirections(cmd);
		char *path = find_command_path(cmd->name, mini->envp->env);
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putendl_fd(cmd->name, 2);
			exit(127);
		}
		execve(path, cmd->args, mini->envp->env);
		perror(cmd->name);
		free(path);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->envp->exit_status = WEXITSTATUS(status);
	debug_fd("After execute_external_command");
}
