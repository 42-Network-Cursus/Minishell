/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmajerus <lmajerus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:29 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/06 17:33:29 by lmajerus         ###   ########.fr       */
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
	if (s1[i] != '=')
		return (0);
	return (1);
}

static	int	var_check_replace(char ***env, char *cmd, int var_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((*env)[i])
	{
		if (comp((*env)[i], cmd, var_len))
		{
			j = 1;
			break ;
		}
		i++;
	}
	if (!j)
		return (1);
	free((*env)[i]);
	(*env)[i] = ft_strdup(cmd);
	if (!(*env)[i])
		exit(0);
	return (0);
}

static	int	export_check(char *cmd)
{
	int	i;

	i = 0;
	if (!ft_isalpha(cmd[i]))
	{
		error_mess("minishell: export: ",
			cmd, ": not a valid indentifier", 1);
		return (0);
	}
	while (cmd[i])
	{
		if (cmd[i] == '=')
			break ;
		if (ft_isspace(cmd[i]))
		{
			error_mess("minishell: export: ",
				cmd, ": not a valid indentifier", 1);
			return (0);
		}
		i++;
	}
	if ((size_t)i == ft_strlen(cmd))
		return (0);
	return (i);
}

int	realloc_env(int j, int i, **ret, **env)
{
	ret[j] = ft_strdup(env[j]);
	if (!ret)
		exit(0);
	free(env[j]);
}

int	ft_export(char ***env, char **cmd)
{
	char	**ret;
	int		i;
	int		j;
	int		k;

	k = 0;
	while (cmd[++k])
	{
		i = 0;
		j = -1;
		if (!export_check(cmd[k])
			|| !var_check_replace(env, cmd[k], export_check(cmd[k])))
			continue ;
		while ((*env)[i])
			i++;
		ret = malloc(sizeof(char *) * (i + 2));
		if (!ret)
			return (exit(0), 1);
		while (++j < i)
		{
			realloc_env(j, i,)
		//	ret[j] = ft_strdup((*env)[j]);
		//	if (!ret)
		//		return (exit(0), 1);
		//	free((*env)[j]);
		}
		ret[i] = ft_strdup(cmd[k]);
		if (!ret)
			return (exit(0), 1);
		ret[++i] = NULL;
		free(*env);
		*env = ret;
	}
	return (1);
}
