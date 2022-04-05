/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:48:45 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 16:40:23 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "builtins.h"
# include "minishell.h"
# include <signal.h>
# include <errno.h>

typedef struct s_pipes
{
	int		new_end[2];
	int		old_end[2];
	int 	f_in;
	int		f_out;
}				t_pipes;

char	*ft_cmd_path(char **env, char *cmd);
int		ft_execute(char **cmd, char **envp); 
void	ft_exec_error(char *str);
int		ft_bin_solo(char **cmd, char ***env, t_here *doc);
int		ft_bin(char ***env, char **cmd, t_pipes p);
void	ft_exec_cmd(t_mini *shell);
void	parent_process(t_mini *shell, t_pipes *p, int i, pid_t pid);
int		ft_bin_heredoc(char **cmd);

//heredoc.c
int		find_heredoc(t_cmd *cmds, int nb_cmd);
void	loop_heredoc(t_cmd *cmds, int nb_cmd);
int		open_heredoc(t_here *doc);
int		delim_is_input(char *input, char *delimiter);

void	unlink_error(void);
void	signal_handler2(int signum);
int		handle_found_delim(t_here *doc, char *input, int fd);

#endif