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

int g_whatsup = 0;

void printmatrix (char **matrix)
{
	int i = 0;
	while (matrix[i])
	{
		printf("%s\n", matrix[i]);
		i++;
	}
}

void	ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dst[i] = '\0';
		i++;
	}
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup(const char *s1)
{
	int		len;
	int		i;
	char	*s2;

	len = 0;
	while (s1[len])
		len++;
	i = 0;
	s2 = (char *)malloc(sizeof(char) * len + 1);
	if (s2 == NULL)
		return (NULL);
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

char **copyenv(char **envp) { //ringraziare chattone che come lui nessuno
    int count = 0;

    // Conta le variabili nell'env originale
    while (envp[count])
        count++;

    // Alloca spazio per la copia
    char **env = malloc(sizeof(char *) * (count + 1));
    if (!env)
        return NULL;

    // Copia ogni stringa
    for (int i = 0; i < count; i++) {
        env[i] = ft_strdup(envp[i]);
        if (!env[i]) {
            // Libera la memoria in caso di errore
            while (i > 0)
                free(env[--i]);
            free(env);
            return NULL;
        }
    }
    env[count] = NULL; // Termina con NULL
    return env;
}

void free_env(char **env) {
    if (!env)
        return;
    for (int i = 0; env[i]; i++)
        free(env[i]);
    free(env);
}

char **export(char **env, const char *new_var) {
    int i = 0;
    int name_len = strchr(new_var, '=') - new_var + 1;

    while (env[i]) {
        // Cerca una variabile con lo stesso nome
        if (strncmp(env[i], new_var, name_len - 1) == 0 && env[i][name_len - 1] == '=') {
            free(env[i]); // Libera la vecchia variabile
            env[i] = strdup(new_var); // Assegna la nuova
            return env;
        }
        i++;
    }

    // Aggiungi una nuova variabile
    char **nenv = realloc(env, sizeof(char *) * (i + 2));
    if (!nenv)
        return NULL;

    nenv[i] = strdup(new_var);
    if (!nenv[i]) {
        free(nenv);
        return NULL;
    }
    nenv[i + 1] = NULL;
    return nenv;
}

void inizializer(t_mini *mini, char **env)
{
	mini->envp = safe_malloc(sizeof(t_env));
	mini->envp->env = copyenv(env);
	mini->envp->exportenv = copyenv(env);
}

int main(int ac, char **av, char **env)
{
	t_mini mini;
	(void)ac;
	(void)av;

	inizializer(&mini, env);		//INIZIALIZZA MINI, COPIA ENV E INIZIALIZZA SHELL
	printmatrix(mini.envp->env);	//STAMPA ENV

}
