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

int	open_heredoc(t_here *doc)
{
	int		fd;
	char	*input;

	fd = open("heredoc", O_CREAT | O_WRONLY, 0666);
	while (1)
	{
		input = readline(">");
		if (*input && !ft_strncmp(input, doc->delimiter, ft_strlen(input))) //SEGV CTRL-D HERE DURING HEREDOC> + NEED TO DEL HEREDOC (file) IF CTRL-D
		{
			if (doc->next == NULL)
			{
				free(input);
				close(fd);
				fd = open("heredoc", O_RDONLY, 0666);
				if (unlink("heredoc"))
					ft_exec_error("Unlink");
				return (fd);
			}
			else
			{
				doc = doc->next;
				close(fd);
				fd = open("heredoc", O_TRUNC | O_WRONLY, 0666);
			}
		}
		else
		{
			write(fd, input, ft_strlen(input));
			write(fd, "\n", 1);
		}
		free(input);
	}
	return (0);
}

void	check_in_out_redir(t_mini *shell, t_pipes *p, int i)
{
	char	*red_inf;
	char	*red_outf;

	p->f_in = 0;
	p->f_out = 1;
	red_inf = shell->cmds[i].redir_in.file_name;
	red_outf = shell->cmds[i].redir_out.file_name;
	if (shell->cmds[i].redir_in.doc)
		p->f_in = open_heredoc(shell->cmds[i].redir_in.doc);
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
		dup2(p->f_out, 1); //CHECK IF ERROR ?
	if (p->f_in != 0)
		dup2(p->f_in, 0); //CHECK IF ERROR ?
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
			error_mess("minishell: ", shell->cmds[i].av[0], ": command not found", 127);
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

	
	ft_bzero(&p, sizeof(t_pipes)); // wat ?
	if (shell->nb_cmd == 1)					// je lance les builtins sans fork si il n'y a pas de pipes
	{
		//NEED TO REDIR IN OUT
		//+ << IN FORK
		//FUCK
		if (ft_bin(&(shell->env), shell->cmds[0].av, p) == 1)
			return ;
	}
	i = -1;
	while (++i < shell->nb_cmd)
	{
		if (i < shell->nb_cmd - 1)
			pipe(p.new_end);
		pid = fork();
		if (pid == -1)
			ft_exec_error("Fork"); //CHANGE
		else if (pid == 0)
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
