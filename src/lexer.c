/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:32:44 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 18:45:58 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_double_quote_content(t_mini *mini,
	const char *input, size_t *i, size_t start)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$' && input[*i + 1]
			&& (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_'
				|| input[*i + 1] == '?'))
		{
			tmp = ft_substr(input, start, *i - start);
			result = ft_strjoin_free(result, tmp);
			tmp = expand_env_var(mini, input, i);
			result = ft_strjoin_free(result, tmp);
			start = *i;
		}
		else
			(*i)++;
	}
	if (*i > start)
		result = ft_strjoin_free(result, ft_substr(input, start, *i - start));
	return (result);
}

static char	*handle_double_quotes(t_mini *mini, const char *input, size_t *i)
{
	char	*result;
	size_t	start;

	(*i)++;
	start = *i;
	result = process_double_quote_content(mini, input, i, start);
	if (!input[*i])
	{
		free(result);
		ft_putendl_fd("minishell: syntax error: unclosed double quote", 2);
		return (NULL);
	}
	(*i)++;
	return (result);
}

static int	handle_quotes_and_vars(t_mini *mini, t_token **tokens,
	const char *input, size_t *i)
{
	char	*content;

	if (input[*i] == '\'')
	{
		content = handle_single_quotes(input, i);
		if (!content)
			return (0);
		add_token(tokens, TOKEN_TEXT, content);
	}
	else if (input[*i] == '"')
	{
		content = handle_double_quotes(mini, input, i);
		if (!content)
			return (0);
		add_token(tokens, TOKEN_TEXT, content);
	}
	else if (input[*i] == '$')
	{
		content = expand_env_var(mini, input, i);
		if (*content)
			add_token(tokens, TOKEN_VAR, content);
		else
			free(content);
	}
	return (1);
}

static void	handle_word(t_token **tokens, const char *input, size_t *i)
{
	char	*word;

	word = get_word(input, i);
	if (word && *word)
		add_token(tokens, TOKEN_WORD, word);
	else
		free(word);
}

t_token	*lexer(t_mini *mini, const char *input)
{
	t_token	*tokens;
	size_t	i;

	tokens = NULL;
	i = 0;
	while (input && input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '"' || input[i] == '$')
		{
			if (!handle_quotes_and_vars(mini, &tokens, input, &i))
				return (free_tokens(tokens), NULL);
		}
		else if (is_special_char(input[i]))
			handle_special_chars(&tokens, input, &i);
		else
			handle_word(&tokens, input, &i);
	}
	return (tokens);
}