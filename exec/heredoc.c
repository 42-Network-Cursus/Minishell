/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwastche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 16:49:09 by cwastche          #+#    #+#             */
/*   Updated: 2022/04/02 16:49:11 by cwastche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "builtins.h"

void	fill_err_var(t_err_msg *err, t_cmd *cmd, int redir)
{
	if (redir == 1)
		err->filename = cmd->redir_in.file_name;
	else if (redir == 2)
		err->filename = cmd->redir_out.file_name;
}

void	loop_heredoc(t_cmd *cmds, int nb_cmd)
{
	int	i;
	int	fd;

	i = -1;
	while (++i < nb_cmd)
	{
		if (cmds[i].redir_in.doc)
		{
			fd = open_heredoc(cmds[i].redir_in.doc);
			close(fd);
		}
	}
}

int	find_heredoc(t_cmd *cmds, int nb_cmd)
{
	int	i;

	if (cmds)
	{
		i = -1;
		while (++i < nb_cmd)
		{
			if (cmds[i].redir_in.doc)
				return (1);
		}
	}
	return (0);
}

int	delim_is_input(char *input, char *delimiter)
{
	int	len;
	int	i;

	len = ft_strlen(input);
	i = -1;
	while (++i < len)
	{
		if (input[i] != delimiter[i])
			return (0);
	}
	if (!input[i] && !delimiter[i])
		return (1);
	return (0);
}


//NOT SURE HOW TO MAKE SIGNALS WORK FOR CTRL-C
//COPY OF MAIN SIGNAL_HANDLER DOES NOT WORK

int	open_heredoc(t_here *doc)
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
		if (!input)
		{
			if (unlink("heredoc"))
				ft_exec_error("Unlink"); //TO REPLACE
			close(fd);
			exit(0);
		}
		if (*input && delim_is_input(input, doc->delimiter))
		{
			if (doc->next == NULL)
			{
				free(input);
				close(fd);
				fd = open("heredoc", O_RDONLY, 0666);
				if (unlink("heredoc"))
					ft_exec_error("Unlink"); //TO REPLACE
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
