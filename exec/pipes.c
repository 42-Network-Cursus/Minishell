/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmajerus <lmajerus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 17:23:12 by cwastche          #+#    #+#             */
/*   Updated: 2022/04/06 17:33:32 by lmajerus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

void	check_in_out_redir(t_mini *shell, t_pipes *p, int i)
{
	char	*r_in;
	char	*r_out;

	p->f_in = 0;
	p->f_out = 1;
	if (shell->cmds[i].redir_in.doc)
		p->f_in = open_heredoc(shell->cmds[i].redir_in.doc);
	r_in = shell->cmds[i].redir_in.file_name;
	r_out = shell->cmds[i].redir_out.file_name;
	if (r_in)
	{
		p->f_in = open(r_in, shell->cmds[i].redir_in.flags);
		if (p->f_in < 0)
			error_mess("minishell: ", r_in, ": No such file or directory", 1);
	}	
	if (r_out)
	{
		p->f_out = open(r_out, shell->cmds[i].redir_out.flags);
		if (p->f_out < 0)
			error_mess("minishell: ", r_out, ": No such file or directory", 1);
	}
}

void	child_process(t_mini *shell, t_pipes *p, int i, char *cmd_path)
{
	int		ret;
	char	*cmd;

	check_in_out_redir(shell, p, i);
	if (p->f_out != 1)
		my_dup(p->f_out, 1);
	if (p->f_in != 0)
		my_dup(p->f_in, 0);
	if (i > 0)
		dup_and_close_pipe(p->old_end[0], p->f_in, p->old_end);
	if (i + 1 < shell->nb_cmd)
		dup_and_close_pipe(p->new_end[1], p->f_out, p->new_end);
	ret = ft_bin(&(shell->env), shell->cmds[i].av, *p);
	if (ret == 1)
		exit(0);
	if (ret == 8)
	{
		ret = execve(cmd_path, shell->cmds[i].av, shell->env);
		if (ret == -1)
		{
			cmd = shell->cmds[i].av[0];
			error_mess("Minishell: ", cmd, ": command not found", 127);
			exit(2);
		}
	}
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
	wait(&g_es);
	if (g_es == 512)
		g_es = 127;
	waitpid(pid, NULL, 0);
}

void	ft_fork(t_mini *shell, t_pipes *p, int i)
{
	char	*cmd_path;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_mess(NULL, "Error forking", NULL, 10);
	else if (pid == 0)
	{
		if (!shell->cmds[i].redir_in.doc)
			signal(SIGQUIT, SIG_DFL);
		if (bin_normalise(&shell->cmds[i].av[0]))
		{
			cmd_path = ft_cmd_path(shell->env, shell->cmds[i].av[0]);
			child_process(shell, p, i, cmd_path);
		}
		exit(0);
	}
	else
		parent_process(shell, p, i, pid);
}

void	ft_exec_cmd(t_mini *shell)
{
	int		i;
	t_pipes	p;

	if (shell->nb_cmd == 1 && check_for_bin(shell))
		return ;
	i = -1;
	while (++i < shell->nb_cmd)
	{
		if (i < shell->nb_cmd - 1)
		{
			if (pipe(p.new_end))
			{
				error_mess("minishell: ", "failed to create pipe", NULL, 1);
				return ;
			}
		}
		if (shell->cmds[i].redir_in.doc)
			signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, signal_handler2);
		ft_fork(shell, &p, i);
	}
}
