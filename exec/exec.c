/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:47:25 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/29 16:48:29 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"
#include "minishell.h"

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

int	ft_bin(char ***env, char **cmd, t_pipes p)
{
	ft_minimize(&cmd);
	if (!ncmp(cmd[0], "echo", 4))
		return (ft_echo(cmd, *env));
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
	{
		write(1, "exit\n", 5);
		return (exit(0), 1);
	}
	return (8);
}