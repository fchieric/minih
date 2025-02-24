/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerquotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:46:54 by fabi              #+#    #+#             */
/*   Updated: 2025/02/24 15:59:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char    *append_to_result(char *result, const char *append)
{
    char    *tmp;

    tmp = ft_strjoin(result, append);
    free(result);
    return (tmp);
}

static char    *process_dollar_var(t_mini *mini, const char *input, 
    size_t *i, char *result)
{
    char    *var_value;
    
    var_value = expand_env_var(mini, input, i);
    result = append_to_result(result, var_value);
    free(var_value);
    return (result);
}

static char    *process_regular_char(const char *input, size_t *i, char *result)
{
    char    *tmp;
    
    tmp = ft_substr(input, *i, 1);
    result = append_to_result(result, tmp);
    free(tmp);
    (*i)++;
    return (result);
}

char    *handle_double_quotes(t_mini *mini, const char *input, size_t *i)
{
    char    *result;

    (*i)++;
    result = ft_strdup("");
    while (input[*i] && input[*i] != '"')
    {
        if (input[*i] == '$' && input[*i + 1] && 
            (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_' 
            || input[*i + 1] == '?'))
            result = process_dollar_var(mini, input, i, result);
        else
            result = process_regular_char(input, i, result);
    }
    if (!input[*i])
    {
        free(result);
        ft_putendl_fd("minishell: syntax error: unclosed double quote", 2);
        return (NULL);
    }
    (*i)++;
    return (result);
}

char	*handle_single_quotes(const char *input, size_t *i)
{
	char	*content;
	size_t	start;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
	{
		ft_putendl_fd("minishell: syntax error: unclosed single quote", 2);
		return (NULL);
	}
	content = ft_substr(input, start, *i - start);
	(*i)++;
	return (content);
}
