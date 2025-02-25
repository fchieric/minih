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
	char	*pwd_path;
	char	*end_prompt;
	char	*shlvl_str;
	int		shlvl;
	char	*level_str;

	shlvl_str = ft_getenv(mini->envp->env, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atol(shlvl_str);
	else
		shlvl = 1;
	if (shlvl > 1)
	{
		level_str = ft_itoa(shlvl); //da mettere ft
		prompt = ft_strjoin(ORANGE"(", level_str);
		free(level_str);
		level_str = ft_strjoin(prompt, ") ");
		free(prompt);
		prompt = level_str;
	}
	else
		prompt = ft_strdup("");
	level_str = ft_strjoin(prompt, "🐚" " "RED "m" ORANGE "i" YELLOW "n" GREEN "i"
		BLUE "s" INDIGO "h" VIOLET "e" RED "l" ORANGE "l"
		INDIGO ": " "🌈 "RESET);
	free(prompt);
	prompt = level_str;
	pwd_path = ft_strdup((const char *)ft_pwd(mini->envp->env));
	pwd_path = ft_strjoin(SLAY, pwd_path);
	end_prompt = ft_strdup(RESET" ""🌈"" " INDIGO "> " RESET);
	level_str = ft_strjoin(prompt, pwd_path);
	free(prompt);
	prompt = level_str;
	level_str = ft_strjoin(prompt, end_prompt);
	free(prompt);
	free(pwd_path);
	free(end_prompt);
	return (level_str);
}

static void	handle_input(t_mini *mini, char *input)
{
	t_token	*tokens;

	if (!input || !*input)
		return ;
	add_history(input);
	tokens = lexer(mini, input);
	if (!tokens)
		return ;
	if (ft_strcmp("exit", tokens->value) == 0)
	{
		printf(EXIT_MSG);
		free_tokens(tokens);
		free(input);
		free_env(mini->envp->env);
		free_env(mini->envp->exportenv);
		free(mini->envp);
		exit(0);
	}
	process_tokens(tokens, mini);
	free_tokens(tokens);
}

static void	shell_loop(t_mini *mini)
{
	char	*input;
	char	*prompt;

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
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	t_mini	mini;

	(void)ac;
	(void)av;
	setup_parent_signals();
	init_shell(&mini, env);
	shell_loop(&mini);
	free_env(mini.envp->env);
	free_env(mini.envp->exportenv);
	free(mini.envp);
	return (0);
}