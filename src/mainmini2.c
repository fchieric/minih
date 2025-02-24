/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainmini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:50:04 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 16:50:04 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_whatsup = 0;

static char	*get_rainbow_prompt(t_mini *mini)
{
	char	*prompt;
	char 	*pivud;
	char	*endprompt;

	prompt = ft_strdup("🐚" " "RED "m" ORANGE "i" YELLOW "n" GREEN "i"
		BLUE "s" INDIGO "h" VIOLET "e" RED "l" ORANGE "l"
		INDIGO ": " "🌈 "RESET);
	pivud= ft_strdup((const char*)ft_pwd(mini->envp->env));
	pivud = ft_strjoin(SLAY, pivud);
	endprompt = ft_strdup(RESET" ""🌈"" " INDIGO "> " RESET);
	prompt = ft_strjoin(prompt, pivud);
	prompt = ft_strjoin(prompt, endprompt);
	free(pivud);
	free(endprompt);
	return (prompt);
}
static void    handle_input(t_mini *mini, char *input)
{
    t_token *tokens;

    if (*input)
    {
        add_history(input);
        tokens = lexer(mini, input);
        if (tokens)
        {
            if (ft_strcmp("exit", tokens->value) == 0)
            {
                printf(EXIT_MSG);
                free_tokens(tokens);
                free(input);
                exit(0);
            }
            process_tokens(tokens, mini);
            free_tokens(tokens);
        }
    }
    free(input);
}

static void    shell_loop(t_mini *mini)
{
    char    *input;
    char    *prompt;

    while (1)
    {
        prompt = get_rainbow_prompt(mini);
        input = readline(prompt);
        free(prompt);
        if (!input)
        {
            printf(EXIT_MSG);
            break ;
        }
        handle_input(mini, input);
    }
}

int	main(int ac, char **av, char **env)
{
	t_mini	mini;
	(void)ac;
	(void)av;

	signal(SIGINT, ctrlc);  // Ctrl+C
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ (ignorato)
	init_shell(&mini, env);
	shell_loop(&mini);
	free_env(mini.envp->env);
	free_env(mini.envp->exportenv);
	free(mini.envp);
	return (0);
}
