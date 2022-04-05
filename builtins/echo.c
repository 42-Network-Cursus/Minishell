/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 17:23:42 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 16:19:59 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

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
	(*v).k = 1;
	(*v).len = 0;
}

void	nl_convert(char ***s)
{
	int		i;

	i = 0;
	while ((*s)[i])
	{
		nl_convert_process(&(*s)[i]);
		i++;
	}
}

static int	echo_process(t_var *v, char **cmd)
{
	if (!ncmp(cmd[1 + (*v).f], "-", 1))
		(*v).k++;
	if (!cmd[(*v).f + (*v).k])
	{
		if (!(*v).f)
			write(1, "\n", 1);
		return (1);
	}
	return (0);
}

int	ft_echo(char **cmd)
{
	t_var	v;

	init(&v);
	nl_convert(&cmd);
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
				write(1, " ", 1);
			v.k++;
		}
	}
	if (!v.f)
		write(1, "\n", 1);
	return (1);
}
