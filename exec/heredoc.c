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

//TO CALL FCT USE
//	if (shell->cmds[i].redir_in.doc)
//		f_tmp = open_heredoc(shell->cmds[i].redir_in.doc);

//TO RUN SOMEWHERE INSIDE PARSER, IN A LOOP BEFORE CHECKING ALL CMDS AND REDIRS

int	open_heredoc(t_here *doc	) //NOT SURE HOW TO MAKE SIGNALS WORK FOR CTRL-C
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
				ft_exec_error("Unlink"); //TO REPLACE
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
