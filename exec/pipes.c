/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 10:47:50 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/02 16:49:27 by cwastche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

// FCT TO DEL, REPLACED BY ERROR_MESS()
void	ft_exec_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}


char	*ft_cmd_path(char **env, char *cmd)
{
	int		i;
	char	**paths;

	i = 0;
	if(cmd[0] == '.' && cmd[1] == '/')
		return (cmd);
	while (!ft_strnstr(env[i], "PATH=", 5) && env[i])
		i++;
	env[i] = ft_substr(env[i], 6, ft_strlen(env[i]));
	paths = ft_split(env[i], ':');
	i = -1;
	while (paths && paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], cmd);
		if (!access(paths[i], F_OK) && !access(paths[i], X_OK))
			return (paths[i]);
		free(paths[i]);
	}
	return (0);
}



void	check_in_out_redir(t_mini *shell, t_pipes *p, int i)
{
	char	*red_inf;
	char	*red_outf;

	p->f_in = 0;
	p->f_out = 1;
	if (shell->cmds[i].redir_in.doc)
		p->f_in = open_heredoc(shell->cmds[i].redir_in.doc);
	red_inf = shell->cmds[i].redir_in.file_name;
	red_outf = shell->cmds[i].redir_out.file_name;
	if (red_inf)
	{
		p->f_in = open(red_inf, shell->cmds[i].redir_in.flags);
		if (p->f_in < 0)
			ft_exec_error("Open"); //TO REPLACE
	}	
	if (red_outf)
	{
		p->f_out = open(red_outf, shell->cmds[i].redir_out.flags);
		if (p->f_out < 0)
			ft_exec_error("Open"); //TO REPLACE
	}
		
}

void	close_pipe(int *end)
{
	close(end[0]); //ADD ERROR HANDLER ?
	close(end[1]); //ADD ERROR HANDLER ?
}

void	child_process(t_mini *shell, t_pipes *p, int i, char *cmd_path)
{
	int		ret;

	check_in_out_redir(shell, p, i);
	if (p->f_out != 1)
		dup2(p->f_out, 1); //CHECK ERROR 
	if (p->f_in != 0)
		dup2(p->f_in, 0); //CHECK ERROR
	if (i > 0)
	{
		dup2(p->old_end[0], p->f_in); // check error
		close_pipe(p->old_end);
	}
	if (i + 1 < shell->nb_cmd)
	{
		dup2(p->new_end[1], p->f_out); //check error
		close_pipe(p->new_end);
	}
	ret = ft_bin(&(shell->env), shell->cmds[i].av, *p);
	if (ret == 1)
		exit(0);
	if (ret == 8)
	{
		if (execve(cmd_path, shell->cmds[i].av, shell->env) == -1)
			error_mess("Minishell: ", shell->cmds[i].av[0], ": command not found", 127);
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
	waitpid(pid, NULL, 0);
}

void	ft_exec_cmd(t_mini *shell)
{
	int 	i;
	pid_t 	pid;
	t_pipes p;
	char	*cmd_path;

	if (shell->nb_cmd == 1)
	{
		if (!bin_normalise(shell->cmds[0].av))
			return ;
		else
			if (ft_bin_solo(shell->cmds[0].av, &shell->env, shell->cmds[0].redir_in.doc) == 1)
				return ;
	}
	i = -1;
	while (++i < shell->nb_cmd)
	{
		if (i < shell->nb_cmd - 1)
			pipe(p.new_end);
		pid = fork();
		if (pid == -1)
			error_mess(NULL, "Error forking", NULL, 10);
		else if (pid == 0)
		{
			if (bin_normalise(&shell->cmds[i].av[0]))
			{
				cmd_path = ft_cmd_path(shell->env, shell->cmds[i].av[0]);
				child_process(shell, &p, i, cmd_path);
			}
			exit(0);
		}
		else
			parent_process(shell, &p, i, pid);
	}
	if (shell->nb_cmd > 1)
		close_pipe(p.old_end);
}
