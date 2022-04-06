/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_bin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmajerus <lmajerus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:47:25 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/06 17:33:31 by lmajerus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

void	ft_minimize(char ***cmd)
{
	int	i;

	i = 0;
	while ((*cmd)[0][i])
	{
		ft_tolower((int)(*cmd)[0][i]);
		i++;
	}
}

int	ft_bin_solo(char **cmd, char ***env, t_here *doc)
{
	t_pipes	p;

	p.f_in = 0;
	p.f_out = 1;
	ft_minimize(&cmd);
	if (ft_bin_heredoc(cmd))
	{
		if (doc)
			open_heredoc(doc);
	}
	if (!ncmp(cmd[0], "cd", 2))
		return (ft_cd(cmd));
	if (!ncmp(cmd[0], "export", 6))
		return (ft_export(env, cmd));
	if (!ncmp(cmd[0], "unset", 5))
		return (ft_unset(env, cmd));
	if (!ncmp(cmd[0], "exit", 4))
		return (ft_exit(cmd));
	if (!ncmp(cmd[0], "env", 3))
		return (ft_env(*env, cmd, p));
	return (0);
}

int	ft_bin(char ***env, char **cmd, t_pipes p)
{
	ft_minimize(&cmd);
	if (!ncmp(cmd[0], "echo", 4))
		return (ft_echo(cmd));
	if (!ncmp(cmd[0], "cd", 2))
		return (ft_cd(cmd));
	if (!ncmp(cmd[0], "pwd", 3))
		return (ft_pwd(cmd, p));
	if (!ncmp(cmd[0], "export", 6))
		return (ft_export(env, cmd));
	if (!ncmp(cmd[0], "unset", 5))
		return (ft_unset(env, cmd));
	if (!ncmp(cmd[0], "env", 3))
		return (ft_env(*env, cmd, p));
	if (!ncmp(cmd[0], "exit", 4))
		return (ft_exit(cmd));
	return (8);
}

int	ft_bin_heredoc(char **cmd)
{
	if (!ncmp(cmd[0], "cd", 2))
		return (1);
	if (!ncmp(cmd[0], "export", 6))
		return (1);
	if (!ncmp(cmd[0], "unset", 5))
		return (1);
	if (!ncmp(cmd[0], "exit", 4))
		return (1);
	return (0);
}
