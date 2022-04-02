/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 10:47:50 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 16:42:28 by mtournay         ###   ########.fr       */
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


// FCT TO DEL, REPLACED BY ERROR_MESS()
void	ft_exec_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int	open_heredoc(t_here *doc) //NOT SURE HOW TO MAKE SIGNALS WORK FOR CTRL-C
{
	int		fd;
	char	*input;

	fd = open("heredoc", O_CREAT | O_WRONLY, 0666);
	while (1)
	{
	//	signal(SIGQUIT, SIG_IGN); 
	//	signal(SIGINT, signal_handler);
		input = readline(">");
	//	signal(SIGQUIT, SIG_IGN);
		if (!input) //CTRL-D CHECK
		{
			if (unlink("heredoc"))
				ft_exec_error("Unlink");
			close(fd);
			exit(0);
		}
		if (*input && !ft_strncmp(input, doc->delimiter, ft_strlen(input)))
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
	int		f_tmp;

	if (shell->cmds[i].redir_in.doc)
		f_tmp = open_heredoc(shell->cmds[i].redir_in.doc);
	if (p->f_in == 0)
		p->f_in = f_tmp;
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


//cd < file == SEGV

void	ft_exec_cmd(t_mini *shell)
{
	int 	i;
	pid_t 	pid;
	t_pipes p;
	char	*cmd_path;

	//Using export | ls, not going through B_IN export cmd, but execve

	//COMMENTED PART DOWN BELOW NEEDED TO UPDATE PWD FROM CD CALLS
	//HOWEVER FUCKS UP BIN NORMALIZE CALLS: /usr/bin/cd OR /bin/ls

	//Example: exit << eof
	//Not launching Heredoc, directly exiting

//	ft_bzero(&p, sizeof(t_pipes)); // Delete ?,a
	if (shell->nb_cmd == 1)					// je lance les builtins sans fork si il n'y a pas de pipes
	{
		//NEED TO REDIR IN OUT
		//+ << IN FORK
		//FUCK
		if (!bin_normalise(shell->cmds[0].av))
			return ;
		else
			if (ft_bin_solo(shell->cmds[0].av, &shell->env) == 1)
				return ;
	}



		i = -1;
		while (++i < shell->nb_cmd)
		{
			check_in_out_redir(shell, &p, i);
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
				exit(0);         // kill the child process if execve does not launch
			}
			else
				parent_process(shell, &p, i, pid);
		}
		if (shell->nb_cmd > 1)
			close_pipe(p.old_end);
}
