/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:53:14 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/24 23:41:28 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_input(const char *filename)
{
	int	fd;

	debug_fd("Before redirect_input");
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return; // Non uscire, segnala solo l'errore
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	debug_fd("After redirect_input");
}

void	redirect_output(const char *filename, int append_mode)
{
	int	flags;
	int	fd;

	debug_fd("Before redirect_output");
	flags = O_WRONLY | O_CREAT;
	if (append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror(filename);
		return; // Non uscire, segnala solo l'errore
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	debug_fd("After redirect_output");
}

void	handle_command_redirection(t_command *cmd, t_token *curr)
{
	if (!cmd || !curr || !curr->next)
		return ;
		
	if (curr->type == TOKEN_REDIR_IN)
		cmd->infile = ft_strdup(curr->next->value);
	else if (curr->type == TOKEN_REDIR_OUT)
		cmd->outfile = ft_strdup(curr->next->value);
	else if (curr->type == TOKEN_HEREDOC)
		cmd->heredoc = ft_strdup(curr->next->value);
	else if (curr->type == TOKEN_APPEND)
		cmd->append = ft_strdup(curr->next->value);
}
