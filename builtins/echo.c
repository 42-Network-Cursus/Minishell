/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 17:23:42 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/28 18:50:22 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"

typedef struct s_var
{
	int	i;
	int	j;
	int	k;
	int	f;
	int	len;
}				t_var;

static void	init(t_var *v)
{
	(*v).f = 0;
	(*v).j = 0;
	(*v).i = 0;
	(*v).k = 0;
	(*v).len = 0;
}

static int	echo_process(t_var *v, char **cmd)
{
	while (cmd[(*v).k + (*v).f])
	{
		nl_convert(&cmd[(*v).k + (*v).f]);
		(*v).k++;
	}
	(*v).k = 1;
	if (!ncmp(cmd[1 + (*v).f], "-", 1))
		(*v).k++;
	if (!cmd[(*v).f + (*v).k])
	{
		if (!(*v).f)
			write(1, "\n", 1); //PIPE NEEDED FOR STDOUT ??
		return (1);
	}
	return (0);
}

int	ft_echo(char **cmd, char **env)
{
	t_var	v;

	(void)env;
	init(&v);
	if (cmd[1])
		if (!ncmp(cmd[1], "-n", 2))
			v.f = 1;
	if (cmd[v.f + 1])
	{
		if (echo_process(&v, cmd))
			return (1);
		while (cmd[v.f + v.k])
		{
			ft_putstr_fd(cmd[v.f + v.k], 1);
			if (cmd[v.f + v.k + 1])
				write(1, " ", 1); // PIPE ?
			v.k++;
		}
	}
	if (!v.f)
		write(1, "\n", 1); // PIPE ?
	return (1);
}
