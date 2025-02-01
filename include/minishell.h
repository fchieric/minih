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
# include <readline/readline.h>    /* Per readline() */
# include <readline/history.h>     /* Per add_history() */
# include <signal.h>

# define RED "\001\033[1;31m\002"
# define ORANGE "\001\033[38;5;208m\002"
# define YELLOW "\001\033[1;33m\002"
# define GREEN "\001\033[1;32m\002"
# define BLUE "\001\033[1;34m\002"
# define INDIGO "\001\033[38;5;93m\002"
# define VIOLET "\001\033[1;35m\002"
# define RESET "\001\033[0m\002"

# define EXIT_MSG RED "❤ " ORANGE "❤ " YELLOW "👋 " \
    RED "T" ORANGE "h" YELLOW "a" GREEN "n" BLUE "k" INDIGO "s " \
    RED "f" ORANGE "o" YELLOW "r " \
    GREEN "u" BLUE "s" INDIGO "i" VIOLET "n" RED "g " \
    ORANGE "M" YELLOW "i" GREEN "n" BLUE "i" "🐚 " \
    VIOLET "👋 " RED "❤ " ORANGE "❤\n" RESET

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
	char	**exportenv;
	int		exit_status;  // status dell'ultimo comando eseguito
}		t_env;

typedef struct s_mini
{
	t_env	*envp;

}		t_mini;

/* Initialization */
void	inizializer(t_mini *mini, char **env);
void	*safe_malloc(size_t size);

/* Environment functions */
char	**export(char **env, const char *new_var);
char	**copyenv(char **envp);
void	free_env(char **env);
char	*get_env_value(char **env, const char *name);

/* Lexer functions */
t_token	*lexer(t_mini *mini, const char *input);
void	add_token(t_token **tokens, t_token_type type, const char *value);
void	free_token(t_token *token);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);
char	*handle_single_quotes(const char *input, size_t *i);
char	*expand_env_var(t_mini *mini, const char *input, size_t *i);
void	handle_special_chars(t_token **tokens, const char *input, size_t *i);
int		is_special_char(char c);
char	*process_var_in_quotes(t_mini *mini, const char *input, size_t *i, char *result);
char	*handle_double_quotes(t_mini *mini, const char *input, size_t *i);
void	handle_redirection(t_token **tokens, const char *input, size_t *i, char type);

/* String utilities */
char	*ft_strdup(const char *s1);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_free(char *s1, char *s2);
size_t	ft_strlen(const char *s);
void	ft_strncpy(char *dst, const char *src, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_isspace(int c);
int		ft_isalnum(int c);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_itoa(int n);
int		ft_strcmp(const char *s1, const char *s2);
void	ctrlc(int sig);
char	*ft_substr(const char *s, unsigned int start, size_t len);
int		is_valid_token_sequence(t_token *tokens);

#endif