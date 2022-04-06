/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 11:44:22 by lmajerus          #+#    #+#             */
/*   Updated: 2022/04/05 19:03:03 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>

# define OTHER 0
# define PIPE 1
# define REDIR_IN 2
# define REDIR_OUT 3
# define SPACE 4

int		g_es;

typedef struct s_token
{
	char			*data;
	int				data_size;
	int				type;
	int				type_size;
	struct s_token	*next;
}			t_token;

typedef struct s_here
{
	char	*delimiter;
	void	*next;
}				t_here;
typedef struct s_red
{
	char	*file_name;
	int		flags;
	t_here	*doc;
}			t_red;

typedef struct s_cmd
{
	int		ac;
	char	**av;
	t_red	redir_in;
	t_red	redir_out;
}			t_cmd;

typedef struct s_mini
{
	t_cmd	*cmds;
	char	**env;
	int		nb_cmd;
}			t_mini;

// parser.c

int		parser(t_mini *shell, char **input);

// parser_utils.c

int		redir(t_red *red, t_token *head);
int		free_cmds(t_cmd *cmds, int nb_cmd);
int		free_tokens(t_token **head);
void	rl_replace_line(const char *text, int clear_undo);

// ft_str_tok.c

int		ft_strtok(char *str, t_token **head, t_mini *shell);

// ft_strtok_utils.c

int		find_next_quotes(char **str);
t_token	*last_token(t_token *head);
void	token_add_back(t_token **head, t_token *new);
t_token	*create_token(t_token tmp, char *str, int size, int x);

// utils.c

int		ft_strncmp_2(const char *s1, const char *s2, size_t n);
size_t	ft_strlen_2(const char *s);
void	ft_putstr_fd_2(char *s, int fd);
int		ft_error(char *error_msg, int perror_flag);
char	*ft_substr_2(char const *s, unsigned int start, size_t len);
char	*ft_strdup_2(const char *s);
void	*ft_memcpy_2(void *dest, const void *src, size_t n);
char	*ft_strtrim_2(char const *s1, char const *set);
void	*ft_calloc_2(size_t nmemb, size_t size);
void	ft_bzero_2(void *s, size_t n);
void	*ft_memset_2(void *s, int c, size_t n);
t_here	*ft_lstnew_2(void *content);
void	ft_lstadd_back_2(t_here **lst, t_here *new);
t_here	*ft_lstlast_2(t_here *lst);
char	*ft_getenv(char *str, char **env);
char	*ft_itoa2(int n);

// check_env_var.c

int		check_env_var(t_token *token, t_mini *shell, int i);
int		exe(t_mini *shell);
int		smlp(char c, int *macro_len);

// main.c

void	signal_handler(int signum);
#endif
