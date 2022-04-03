/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 17:23:42 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 19:19:31 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

//1.
//Problem?
//BASH echo 1 2\\n 3 -> Output 1 2\n 3
//				Minishell Output 1 2
//								  3
//In pdf: Shell should not interpret unclosed quotes or special characters which are not required by the
//	subject such as \ (backslash)
// Not a problem donc ?

//2.
//IMPLEMENT $? INSIDE ECHO

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

static int	is_nl_end(char **s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i + 1])
		i++;
	while (s[i][j])
		j++;
	if (s[i][j - 1] == '\n')
		return (1);
	return (0);
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
	if (v.f && !is_nl_end(cmd))
		write(1, "%%", 1);
	if (v.f && is_nl_end(cmd))
		return (1);
	write(1, "\n", 1);
	return (1);
}
