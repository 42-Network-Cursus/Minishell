/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:29 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 16:56:56 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

static int	comp(char *s1, char *s2, int len)
{
	int	i;

	i = -1;
	while (s1[++i] && s2[i] && len--)
		if (s1[i] != s2[i])
			return (0);
	return (1);
}

static	int	var_check_replace(char ***env, char **cmd, int var_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((*env)[i])
	{
		if (comp((*env)[i], cmd[1], var_len))
		{
			j = 1;
			break ;
		}
		i++;
	}
	if (!j)
		return (1);
	free((*env)[i]);
	(*env)[i] = ft_strdup(cmd[1]);
	if (!(*env)[i])
		exit(0);
	return (0);
}

static	int	export_check(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd[1])
	{
		write(1, "export: arg required\n", 21);
		*ges = 1;
		return (0);
	}
	while (cmd[1][i])
	{
		if (cmd[1][i] == '=')
			break ;
		i++;
	}
	if ((size_t)i == ft_strlen(cmd[1]))
		return (0);
	return (i);
}

int	ft_export(char ***env, char **cmd)
{
	char	**ret;
	int		i;
	int		j;

	i = 0;
	j = -1;
	if (!export_check(cmd))
		return (1);
	if (!var_check_replace(env, cmd, export_check(cmd)))
		return (1);
	while ((*env)[i])
		i++;
	ret = malloc(sizeof(char *) * (i + 2));
	if (!ret)
		return (exit(0), 1);
	while (++j < i)
		ret[j] = (*env)[j];
	ret[i] = ft_strdup(cmd[1]);
	if (!ret[i])
		return (0);
	i++;
	ret[i] = NULL;
	free(*env);
	*env = ret;
	return (1);
}
