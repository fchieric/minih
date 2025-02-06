/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 21:26:01 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 23:31:11 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ctrlc(int sig)
{
    (void)sig;
    if (!g_whatsup)  // Se siamo nel prompt
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else  // Se c'è un processo in esecuzione
        write(1, "\n", 1);
}

void    sigquit_handler(int sig)
{
    if (g_whatsup)
    {
        ft_putstr_fd("Quit (core dumped)\n", 2);
    }
    (void)sig;
}

// Configura i signal handler per il processo padre
void    setup_parent_signals(void)
{
    signal(SIGINT, ctrlc);
    signal(SIGQUIT, sigquit_handler);
}

// Configura i signal handler per i processi figli
void    setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

// Termina i processi figli
void    terminate_child_processes(t_pipe_state *state, int sig)
{
    int i;

    if (!state)
        return;

    i = 0;
    while (i < state->cmd_count)
    {
        if (state->child_pids[i] > 0)
            kill(state->child_pids[i], sig);
        i++;
    }
    g_whatsup = 0;
}
