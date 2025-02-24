/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:15:09 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/24 22:33:17 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

static int	create_heredoc_file(void)
{
	int	fd;

	fd = open(HEREDOC_TMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	return (fd);
}

static void	read_until_delimiter(int fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line);
	}
}

int	handle_heredoc(const char *delimiter)
{
	int	write_fd;
	int	read_fd;

	write_fd = create_heredoc_file();
	if (write_fd == -1)
		return (-1);
	read_until_delimiter(write_fd, delimiter);
	close(write_fd);
	read_fd = open(HEREDOC_TMP, O_RDONLY);
	if (read_fd == -1)
	{
		perror("minishell: heredoc");
		unlink(HEREDOC_TMP);
		return (-1);
	}
	unlink(HEREDOC_TMP);
	return (read_fd);
}

void	setup_heredoc(t_command *cmd)
{
	int	heredoc_fd;

	if (!cmd->heredoc)
		return ;
	heredoc_fd = handle_heredoc(cmd->heredoc);
	if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
}
