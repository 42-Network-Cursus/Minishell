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

int	handle_found_delim(t_here *doc, t_doc_fd *t_doc)
{
	char	*msg;

	msg = ": No such file or directory";
	if (doc->next == NULL)
	{
		free(t_doc->input);
		close(t_doc->fd);
		t_doc->fd = open("heredoc", O_RDONLY, 0666);
		if (t_doc->fd < 0)
			error_mess("minishell: ", "heredoc", msg, 1);
		if (unlink("heredoc"))
			unlink_error();
		return (1);
	}
	else
	{
		doc = doc->next;
		close(t_doc->fd);
		t_doc->fd = open("heredoc", O_TRUNC | O_WRONLY, 0666);
		if (t_doc->fd < 0)
			error_mess("minishell: ", "heredoc", msg, 1);
	}
	return (0);
}

int	open_heredoc(t_here *doc)
{
	t_doc_fd	t_doc;

	t_doc.fd = open("heredoc", O_CREAT | O_WRONLY, 0666);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		t_doc.input = readline(">");
		if (!t_doc.input)
		{
			if (unlink("heredoc"))
				unlink_error();
			close(t_doc.fd);
			exit(0);
		}
		if (*t_doc.input && delim_is_input(t_doc.input, doc->delimiter))
		{
			if (handle_found_delim(doc, &t_doc))
				return (t_doc.fd);
		}
		else
			ft_putendl_fd(t_doc.input, t_doc.fd);
		free(t_doc.input);
	}
	return (0);
}
