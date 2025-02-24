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

static void	execute_builtin(t_token *tmp_token, t_mini *mini, t_command *cmd)
{
	tmp_token = safe_malloc(sizeof(t_token));
	if (!tmp_token)
		return ;
	tmp_token->type = TOKEN_WORD;
	tmp_token->value = ft_strdup(cmd->args[0]);
	tmp_token->next = NULL;
	if (cmd->infile)
		redirect_input(cmd->infile);
	if (cmd->outfile)
		redirect_output(cmd->outfile, 0);
	if (cmd->append)
		redirect_output(cmd->append, 1);
	handleword(tmp_token, mini);
	free(tmp_token->value);
	free(tmp_token);
}

void	execute_commands(t_command *cmd, t_mini *mini)
{
	t_token	*tmp_token;

	tmp_token = NULL;
	if (!cmd)
		return ;
	if (!cmd->next)
	{
		if (cmd->type == CMD_BUILTIN)
			execute_builtin(tmp_token, mini, cmd);
		else if (cmd->type == CMD_EXTERNAL)
			execute_external_command(cmd, mini);
	}
	else
		execute_commands_with_pipes(cmd, mini);
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
		execvp(cmd->name, cmd->args);
		perror(cmd->name);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->envp->exit_status = WEXITSTATUS(status);
}
