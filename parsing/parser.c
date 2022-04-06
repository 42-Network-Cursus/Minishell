/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:07:30 by lmajerus          #+#    #+#             */
/*   Updated: 2022/04/05 19:03:42 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

int	get_nb_cmd(t_token *head)
{
	int	nb;

	nb = 1;
	while (head)
	{
		if (head->type == PIPE)
			nb++;
		head = head->next;
	}
	return (nb);
}

static void	fill_ac(t_mini *shell, t_token *head)
{
	int		i;
	t_token	*prev;

	i = 0;
	while (i < shell->nb_cmd)
	{
		prev = head;
		shell->cmds[i].ac = 0;
		while (head && head->type != PIPE)
		{
			if (head->type == OTHER && prev->type != REDIR_IN
				&& prev->type != REDIR_OUT)
				shell->cmds[i].ac++;
			prev = head;
			head = head->next;
		}
		if (head)
			head = head->next;
		i++;
	}
}

static void	set_names_to_null(t_mini *shell)
{
	int	i;

	i = 0;
	while (i < shell->nb_cmd)
	{
		shell->cmds[i].redir_in.file_name = NULL;
		shell->cmds[i].redir_out.file_name = NULL;
		shell->cmds[i].redir_out.flags = 0;
		shell->cmds[i].redir_in.flags = 0;
		shell->cmds[i].redir_in.doc = NULL;
		i++;
	}
}

static int	create_tab_cmd(t_token *head, t_cmd *cmds, int i, int j)
{
	while (head)
	{
		cmds[i].av = malloc(sizeof(char *) * (cmds[i].ac + 1));
		if (!cmds[i].av)
			exit(1);
		j = 0;
		cmds[i].av[cmds[i].ac] = NULL;
		while (head && head->type != PIPE)
		{
			if (head->type == OTHER)
				cmds[i].av[j++] = ft_strdup_2(head->data);
			else if (head->type == REDIR_IN && redir(&cmds[i].redir_in, head))
				;
			else if (head->type == REDIR_OUT && redir(&cmds[i].redir_out, head))
				;
			if (head->type == REDIR_IN || head->type == REDIR_OUT)
				head = head->next;
			if (head)
				head = head->next;
		}
		if (head)
			head = head->next;
		i++;
	}
	return (0);
}

int	parser(t_mini *shell, char **input)
{
	t_token		*head;
	pid_t		pid;

	head = NULL;
	if (!ft_strtok(*input, &head, shell))
		return (ft_error("Syntax error\n", 0));
	shell->nb_cmd = get_nb_cmd(head);
	shell->cmds = malloc(sizeof(t_cmd) * shell->nb_cmd);
	if (!shell->cmds)
		return (0);
	fill_ac(shell, head);
	set_names_to_null(shell);
	create_tab_cmd(head, shell->cmds, 0, 0);
	if (find_heredoc(shell->cmds, shell->nb_cmd) && !shell->cmds[0].av[0])
	{
		signal(SIGINT, signal_handler2);
		pid = fork();
		if (pid == 0)
			loop_heredoc(shell->cmds, shell->nb_cmd);
		waitpid(pid, 0, 0);
	}
	return (free_tokens(&head));
}
