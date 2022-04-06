/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:29 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/06 17:07:03 by mtournay         ###   ########.fr       */
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
		printf("index = %d\n", i);
		// printf("%s\n", (*env)[i]);
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
	// printf("la bas %s\n", (*env)[i]);
	if (!(*env)[i])
		exit(0);
	return (0);
}

static	int	export_check(char *cmd)
{
	int	i;

	i = 0;
	// if (!cmd[i])
	// {
	// 	write(1, "export: arg required\n", 21);
	// 	g_es = 1;
	// 	return (0);
	// }
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

int	ft_export(char ***env, char **cmd)
{
	char	**ret;
	int		i;
	int		j;
	int		k;

	i = 0;
	k = 0;
	j = -1;
	while (cmd[++k])
	{
		if (!export_check(cmd[k]))
			continue ;
		if (!var_check_replace(env, cmd[k], export_check(cmd[k])))
			continue ;
		// printf("ici [%s]\n", cmd[k]);
		while ((*env)[i])
			i++;
		ret = malloc(sizeof(char *) * (i + 2));
		if (!ret)
			return (exit(0), 1);
		printf("i = %d\n", i);
		while (++j < i)
			ret[j] = ft_strdup((*env)[j]);
		ret[i] = ft_strdup(cmd[k]);
		if (!ret[i])
			return (exit(0), 1);
		i++;
		ret[i] = NULL;
		j = 0;
		while (j < i)
			free((*env[j]));
		*env = ret;
	}
	return (1);
}
