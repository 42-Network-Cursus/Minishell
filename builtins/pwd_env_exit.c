/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmajerus <lmajerus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:25 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/06 17:33:30 by lmajerus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

static int	ft_isnum(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < 48 || s[i] > 57)
			return (1);
		i++;
	}
	return (0);
}

int	ft_exit(char **cmd)
{
	if (cmd[1])
	{
		if (ft_isnum(cmd[1]))
			error_mess("minishell: exit: ", cmd[1],
				": numeric argument required", 1);
	}
	write(1, "exit\n", 5);
	return (exit(0), 1);
}

void	get_cd(char **buf)
{
	int		i;

	i = 1;
	*buf = malloc(sizeof(char) * i);
	if (!buf)
		return (exit(0));
	while (!getcwd(*buf, i))
	{
		free(*buf);
		i++;
		*buf = malloc(sizeof(char) * i);
		if (!*buf)
			return (exit(0));
	}
}

int	ft_pwd(char **cmd, t_pipes p)
{
	char	*buf;

	(void)cmd;
	get_cd(&buf);
	write(p.f_out, buf, ft_strlen(buf));
	write(p.f_out, "\n", 1);
	free(buf);
	return (1);
}

int	ft_env(char **env, char **cmd, t_pipes p)
{
	int	i;
	int	len;

	if (cmd[1])
	{
		write(2, "env: ", 5);
		return (error_mess(NULL, cmd[1], ": No such file or directory", 127));
	}
	i = 0;
	while (env[i])
	{
		len = ft_strlen(env[i]);
		write(p.f_out, env[i], len);
		write(p.f_out, "\n", 1);
		i++;
	}
	return (1);
}
