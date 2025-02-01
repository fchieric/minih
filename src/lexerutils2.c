/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:45:14 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 18:55:55 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_var_name(const char *input, size_t *i)
{
	size_t	start;
	char	*var_name;

	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	if (!var_name)
		return (NULL);
	return (var_name);
}

//static char	*handle_exit_status(t_mini *mini)
//{
//	return (ft_itoa(mini->envp->exit_status));
//}

static char	*expand_env_var(t_mini *mini, const char *input, size_t *i)
{
	char	*var_name;
	char	*value;
	char	*result;

	var_name = get_env_var_name(input, i);
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (handle_exit_status(mini));
	}
	value = get_env_value(mini->envp->env, var_name);
	free(var_name);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	return (result);
}

static char	*get_word(const char *input, size_t *i)
{
	size_t	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i])
		&& input[*i] != '\'' && input[*i] != '\"' && input[*i] != '$')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}