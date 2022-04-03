/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:07:30 by lmajerus          #+#    #+#             */
/*   Updated: 2022/03/31 16:07:12 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "builtins.h"

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

static int	create_tab_cmd(t_token *head, t_cmd *cmds, t_err_msg *er)
{
	while (head)
	{
		cmds[er->i].av = malloc(sizeof(char *) * (cmds[er->i].ac + 1));
		if (!cmds[er->i].av)
			exit(1);
		cmds[er->i].av[cmds[er->i].ac] = NULL;
		while (head && head->type != PIPE)
		{
			if (head->type == OTHER)
				cmds[er->i].av[er->j++] = ft_strdup_2(head->data);
			else if (head->type == REDIR_IN && redir(&cmds[er->i].redir_in, head, er))
				return (fill_err_var(er, &cmds[er->i], 1), 1);
			else if (head->type == REDIR_OUT && redir(&cmds[er->i].redir_out, head, er))
				return (fill_err_var(er, &cmds[er->i], 2), 1);
			if (head->type == REDIR_IN || head->type == REDIR_OUT)
				head = head->next;
			if (head)
				head = head->next;
		}
		if (head)
			head = head->next;
		er->i++;
	}
	return (0);
}

int	parser(t_mini *shell, char **input)
{
	t_token		*head;
	t_err_msg	err;

	err.i = 0;
	err.j = 0;
	head = NULL;
	if (!ft_strtok(*input, &head, shell))
		return (ft_error("Syntax eror\n", 0));
	shell->nb_cmd = get_nb_cmd(head);
	shell->cmds = malloc(sizeof(t_cmd) * shell->nb_cmd);
	if (!shell->cmds)
		return (0);
	fill_ac(shell, head);
	set_names_to_null(shell);
	if (create_tab_cmd(head, shell->cmds, &err))
	{
		if (find_heredoc(shell->cmds, shell->nb_cmd))
			loop_heredoc(shell->cmds, shell->nb_cmd);
		open(err.filename, err.flags, 0644);
		ft_error(err.filename, 1);
		free_cmds(shell->cmds, shell->nb_cmd);
		return (free_tokens(&head), 0);
	}
	return (free_tokens(&head));
}
