/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:15:09 by fabi              #+#    #+#             */
/*   Updated: 2025/02/24 16:04:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void write_to_heredoc(int fd, char *line)
{
    write(fd, line, ft_strlen(line));
    write(fd, "\n", 1);
    free(line);
}

static int open_heredoc_fd(void)
{
    int heredoc_fd;

    heredoc_fd = open(HEREDOC_TMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (heredoc_fd == -1)
    {
        perror("minishell: heredoc");
        return (-1);
    }
    return (heredoc_fd);
}

static int read_heredoc_input(int heredoc_fd, const char *delimiter)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write_to_heredoc(heredoc_fd, line);
    }
    close(heredoc_fd);
    return (0);
}

int handle_heredoc(const char *delimiter)
{
    int heredoc_fd;
    int fd;

    heredoc_fd = open_heredoc_fd();
    if (heredoc_fd == -1)
        return (-1);
    read_heredoc_input(heredoc_fd, delimiter);
    fd = open(HEREDOC_TMP, O_RDONLY);
    if (fd == -1)
    {
        perror("minishell: heredoc");
        unlink(HEREDOC_TMP);
        return (-1);
    }
    unlink(HEREDOC_TMP);
    return (fd);
}

void setup_heredoc(t_command *cmd)
{
    int heredoc_fd;

    if (!cmd->heredoc)
        return;

    heredoc_fd = handle_heredoc(cmd->heredoc);
    if (heredoc_fd != -1)
    {
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }
}
