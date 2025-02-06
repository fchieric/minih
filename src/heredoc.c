/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:15:09 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 23:15:25 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_heredoc(const char *delimiter)
{
    char    *line;
    int     fd;
    int     heredoc_fd;

    // Crea un file temporaneo per l'heredoc
    heredoc_fd = open(HEREDOC_TMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (heredoc_fd == -1)
    {
        perror("minishell: heredoc");
        return (-1);
    }

    // Leggi l'input fino al delimiter
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

        // Scrivi la linea nel file temporaneo
        write(heredoc_fd, line, ft_strlen(line));
        write(heredoc_fd, "\n", 1);
        free(line);
    }

    close(heredoc_fd);

    // Riapri il file in lettura
    fd = open(HEREDOC_TMP, O_RDONLY);
    if (fd == -1)
    {
        perror("minishell: heredoc");
        unlink(HEREDOC_TMP);
        return (-1);
    }

    // Rimuovi il file temporaneo
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
        // Reindirizza l'input standard al file dell'heredoc
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }
}
