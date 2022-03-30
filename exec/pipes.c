/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 10:47:50 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/24 12:55:32 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"


char	*ft_cmd_path(char **env, char *cmd)
{
	int		i;
	char	**paths;

	i = 0;
	while (!ft_strnstr(env[i], "PATH=", 5))
		i++;
	env[i] = ft_substr(env[i], 6, ft_strlen(env[i]));
	paths = ft_split(env[i], ':');
	i = -1;
	while (paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], cmd);
		if (!access(paths[i], F_OK) && !access(paths[i], X_OK))
			return (paths[i]);
		free(paths[i]);
	}
	return (0);
}


// FCT TO DEL, REPLACED BY ERROR_MESS()
void	ft_exec_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	check_in_out_redir(t_mini *shell, t_pipes *p, int i)
{
	char	*red_inf;
	char	*red_outf;

	red_inf = shell->cmds[i]redir_in.file_name;
	red_outf = shell->cmds[i].redir_out.file_name;
	if (red_inf)
	{
		p->f_in = open(red_inf, shell->cmds[i].redir_in.flags);
		if (p->f_in < 0)
			ft_exec_error("Open"); //TO REPLACE
	}
	else
		p->f_in = 0;
	if (red_outf)
	{
		p->f_out = open(red_outf, shell->cmds[i].redir_out.flags);
		if (p->f_out < 0)
			ft_exec_error("Open"); //TO REPLACE
	}
	else
		p->f_out = 1;
}

void	close_pipe(int *end)
{
	close(end[0]); //ADD ERROR HANDLER ?
	close(end[1]); //ADD ERROR HANDLER ?
}

void	child_process(t_mini *shell, t_pipes *p, int i, char *cmd_path)
{
	int		ret;

	if (p->f_out != 1)
		dup2(p->f_out, 1);
	if (p->f_in != 0)
		dup2(p->f_in, 0);
	if (i > 0)
	{
		dup2(p->old_end[0], p->f_in);
		close_pipe(p->old_end);
	}
	if (i + 1 < shell->nb_cmd)
	{
		dup2(p->new_end[1], p->f_out);
		close_pipe(p->new_end);
	}
	ret = ft_bin(&(shell->env), shell->cmds[i].av, *p);
	if (ret == 1)
		exit(0);
	if (ret == 8)
	{
		if (execve(cmd_path, shell->cmds[i].av, shell->env) == -1)
			error_mess("minishell: ", shell->cmds[i].av[0], ": command not found", 127);
}

void	parent_process(t_mini *shell, t_pipes *p, int i, pid_t pid)
{
	if (i > 0)
		close_pipe(p->old_end);
	if (i + 1 < shell->nb_cmd)
	{
		p->old_end[0] = p->new_end[0];
		p->old_end[1] = p->new_end[1];
	}
	waitpid(pid, NULL, 0);
}

void	ft_exec_cmd(t_mini *shell)
{
	int 	i;
	pid_t 	pid;
	t_pipes p;
	char	*cmd_path;



//	ft_bzero(&p, sizeof(t_pipes));
	//	if (shell->nb_cmd == 1)					// je lance les builtins sans fork si il n'y a pas de pipes
//	{
//		if (ft_bin(&(shell->env), shell->cmds[0].av, p) == 1)//cmd[i]
//			return ;
//	}

	i = -1;
	while (++i < shell->nb_cmd)
	{
		if (i < shell->nb_cmd - 1)
			pipe(p.new_end);
		check_in_out_redir(shell, &p, i);
		pid = fork();
		if (pid == 0)
		{
			cmd_path = ft_cmd_path(shell->env, shell->cmds[i].av[0]);
			child_process(shell, &p, i, cmd_path);
		}
		else
			parent_process(shell, &p, i, pid);
	}
	if (shell->nb_cmd > 1)
		close_pipe(p.old_end);
}