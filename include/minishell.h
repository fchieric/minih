/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:39:08 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 16:39:08 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

extern int g_whatsup; //variabile globale che aggiorna lo status della shell e segnala se è in esecuzione o meno


typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_TEXT,
	TOKEN_VAR,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
}		t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}		t_token;

typedef struct s_env
{
	char	**env;
	char	**exportenv; // export is a copy of env
}		t_env;

typedef struct s_mini
{
	t_env	*envp;

}		t_mini;


void inizializer(t_mini *mini, char **env);
char	*ft_strdup(const char *s1);
void	*safe_malloc(size_t size);
void printmatrix (char **matrix);
void	ft_strncpy(char *dst, const char *src, size_t n);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char **export(char **env, const char *new_var);
char **copyenv(char **envp);
void free_env(char **env);
t_token	*lexer(t_mini *mini, const char *input);
void	add_token(t_token **tokens, t_token_type type, char *value);




#endif