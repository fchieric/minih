/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:43:38 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 19:43:38 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_token(t_mini *mini, t_token **tokens,
	const char *input, size_t *i)
{
	char	*content;

	if (input[*i] == '\'')
	{
		content = handle_single_quotes(input, i);
		if (!content)
			return (0);
		add_token(tokens, TOKEN_TEXT, content);
		free(content);
	}
	else if (input[*i] == '"')
	{
		content = handle_double_quotes(mini, input, i);
		if (!content)
			return (0);
		add_token(tokens, TOKEN_TEXT, content);
		free(content);
	}
	else if (input[*i] == '$')
	{
		content = expand_env_var(mini, input, i);
		if (content && *content)
			add_token(tokens, TOKEN_VAR, content);
		free(content);
	}
	else
		return (0);
	return (1);
}

static int	is_word_char(char c)
{
	return (!ft_isspace(c) && !is_special_char(c)
		&& c != '\'' && c != '"' && c != '$' && c);
}

static char	*get_next_word(const char *input, size_t *i)
{
	size_t	start;
	size_t	len;
	char	*word;

	if (!input || !input[*i])
		return (NULL);
	start = *i;
	len = 0;
	while (is_word_char(input[*i]))
	{
		len++;
		(*i)++;
	}
	word = ft_substr(input, start, len);
	if (!word)
		return (NULL);
	return (word);
}

static void	handle_word(t_token **tokens, const char *input, size_t *i)
{
	char	*word;

	word = get_next_word(input, i);
	if (word && *word)
		add_token(tokens, TOKEN_WORD, word);
	free(word);
}

t_token	*lexer(t_mini *mini, const char *input)
{
	t_token	*tokens;
	size_t	i;

	tokens = NULL;
	if (!input)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '"' || input[i] == '$')
		{
			if (!process_token(mini, &tokens, input, &i))
			{
				free_tokens(tokens);
				return (NULL);
			}
		}
		else if (is_special_char(input[i]))
			handle_special_chars(&tokens, input, &i);
		else
			handle_word(&tokens, input, &i);
	}
	if (!is_valid_token_sequence(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
