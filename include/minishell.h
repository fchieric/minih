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
#include <fcntl.h>      /* Per O_RDONLY, O_WRONLY, O_CREAT, ecc. */
#include <sys/types.h>  /* Per mode_t */
#include <sys/stat.h>   /* Per mode_t */
#include <sys/wait.h>

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

//per dare un senso ai token word

typedef enum e_cmd_type
{
    CMD_BUILTIN,
    CMD_EXTERNAL,
    CMD_NONE
}   t_cmd_type;

typedef struct s_command
{
    char            *name;         // nome del comando
    char            **args;        // array di argomenti (inclusi flag)
    char            **flags;       // array di flag
    t_cmd_type      type;         // tipo di comando
    char            *infile;       // file di input (<)
    char            *outfile;      // file di output (>)
    char            *heredoc;      // delimititatore heredoc (<<)
    char            *append;       // file append (>>)
    struct s_command *next;        // prossimo comando (dopo pipe)
}   t_command;

typedef struct s_redirs
{
	int		in_fd;
	int		out_fd;
	char	*heredoc_del;
	int		append_mode;
}	t_redirs;

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
int		handleword(t_token *token, t_mini *mini);
int		builtin(char *line);
void	builtinexe(t_token *token, char **envp);
void	process_tokens(t_token *token, t_mini *mini);


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
void	printmatrix(char **matrix);

//per i comandi

t_command    *parse_tokens(t_token *tokens);
t_command    *init_command(void);
void        free_command(t_command *cmd);
void        free_commands(t_command *cmds);
int         is_builtin(const char *cmd);
char        **add_to_array(char **arr, char *str);

void handle_command_redirection(t_command *cmd, t_token *curr);


/* Command execution functions */
int     execute_command(t_command *cmd, t_mini *mini);
int     execute_builtin(t_command *cmd, t_mini *mini);
int     execute_external(t_command *cmd, t_mini *mini);
char    *find_command_path(const char *cmd, char **env);
void    setup_redirections(t_command *cmd);

/* Builtin commands */
int     ft_echo(char **args);
int     ft_cd(char **args, char **env);
void     ft_pwd(char **env);
int     ft_export(char **args, t_mini *mini);
int     ft_unset(char **args, t_mini *mini);
int     ft_env(char **env);
int     ft_exit(char **args, t_mini *mini);

/* Utils functions */
char    **ft_split(const char *s, char c);
void    free_commands(t_command *cmd);

void execute_commands(t_command *cmd, t_mini *mini);
void execute_external_command(t_command *cmd, t_mini *mini);
void redirect_input(const char *filename);
void redirect_output(const char *filename, int append_mode);

void	free_matrix(char **matrix);

#endif