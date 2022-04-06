/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 16:49:09 by cwastche          #+#    #+#             */
/*   Updated: 2022/04/06 16:44:03 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

void	loop_heredoc(t_cmd *cmds, int nb_cmd)
{
	int	i;
	int	fd;

	signal(SIGINT, signal_handler2);
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

int	handle_found_delim(t_here **doc, char *input, int *fd)
{
	char	*msg;

	msg = ": No such file or directory";
	if ((*doc)->next == NULL)
	{
		free(input);
		close(*fd);
		*fd = open("heredoc", O_RDONLY, 0666);
		if (*fd < 0)
			error_mess("minishell: ", "heredoc", msg, 1);
		if (unlink("heredoc"))
			unlink_error();
		return (1);
	}
	else
	{
		*doc = (*doc)->next;
		close(*fd);
		*fd = open("heredoc", O_TRUNC | O_WRONLY, 0666);
		if (*fd < 0)
			error_mess("minishell: ", "heredoc", NULL, 1);
	}
	return (0);
}

int	open_heredoc(t_here *doc)
{
	int		fd;
	char	*input;

	fd = open("heredoc", O_CREAT | O_WRONLY, 0666);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		input = readline(">");
		if (!input)
		{
			if (unlink("heredoc"))
				unlink_error();
			close(fd);
			exit(0);
		}
		if (*input && delim_is_input(input, doc->delimiter))
		{
			if (handle_found_delim(&doc, input, &fd))
				return (fd);
		}
		else
			ft_putendl_fd(input, fd);
		free(input);
	}
	return (0);
}
