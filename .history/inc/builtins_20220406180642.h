/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 16:03:17 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/06 16:44:03 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <signal.h>
# include "minishell.h"
# include "exec.h"

typedef struct s_count
{
	int		i;
	int		j;
	int		k;
}				t_count;

void	get_cd(char **buf);
int		ft_pwd(char **cmd, t_pipes p);
int		ft_cd(char **cmd);
int		ft_echo(char **cmd);
int		ft_unset(char ***env, char **cmd);
int		ft_export(char ***env, char **cmd);
int		ft_env(char **env, char **cmd, t_pipes p);
int		ft_exit(char **cmd);

int		cmp(char *s1, char *s2, int n);
int		ncmp(char *s1, char *s2, size_t n);
void	nl_convert(char ***s);
void	nl_convert_process(char **s);
int		bin_normalise(char **str);

int		error_mess(char *s, char *mess, char *cmd, int exstat);
int		bin_cmd(char ***env, char **cmd);

char	*cat_dup(char *dst, char *src);
void	realloc_env(int j, char **ret, char **env);
void	count_val(t_count *c);

#endif