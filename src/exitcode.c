#include "minishell.h"

void ft_exit(int exit_code, int shouldprint)
{
    char *exit_str;

    exit_str = ft_itoa(exit_code);
    if (exit_str && shouldprint == 1)
    {
        ft_putstr_fd(exit_str, 2);
        ft_putstr_fd("\n", 2);
        free(exit_str);
    }
    g_whatsup = exit_code;
}