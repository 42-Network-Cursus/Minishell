/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:25 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 16:31:42 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

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
	int		i;

	(void)p;
	i = 0;
	while (cmd[i])
		i++;
	if (i > 1)
		return (error_mess(NULL, cmd[0], ": too many arguments", 1));
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

	(void)p;
	if (cmd[1])
	{
		write(1, "env: ", 5);
		return (error_mess(NULL, cmd[1], ": No such file or directory", 127));
	}
	i = 0;
	while (env[i])
	{
		len = ft_strlen(env[i]);
		// printf("f_in: %d\n", p.f_in);
		// printf("f_out: %d\n", p.f_out);
		write(p.f_out, env[i], len);
		write(p.f_out, "\n", 1);
		i++;
	}
	return (1);
}
