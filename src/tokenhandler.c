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
	t_token	*token_chain;
	int		saved_stdout;
	int		saved_stdin;

	if (!cmd)
		return ;
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
			token_chain = create_token_from_cmd(cmd);
			if (token_chain)
			{
				handleword(token_chain, mini);
				free_tokens(token_chain);
			}
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
}

void	setup_redirections(t_command *cmd)
{
	if (cmd->infile)
		redirect_input(cmd->infile);
	if (cmd->outfile)
		redirect_output(cmd->outfile, 0);
	if (cmd->append)
		redirect_output(cmd->append, 1);
	if (cmd->heredoc)
		setup_heredoc(cmd);
}

void	execute_external_command(t_command *cmd, t_mini *mini)
{
	pid_t	pid;
	int		status;
	char	*path;

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
		path = find_command_path(cmd->name, mini->envp->env);
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putendl_fd(cmd->name, 2);
			g_whatsup = 127;
			exit(127);
		}
		execve(path, cmd->args, mini->envp->env);
		perror(cmd->name);
		free(path);
		g_whatsup = 126;
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->envp->exit_status = WEXITSTATUS(status);
	g_whatsup = 0;
}