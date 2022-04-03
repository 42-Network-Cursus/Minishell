/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:29 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 16:31:39 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

int	ft_export(char ***env, char **cmd)
{
	char	**ret;
	int		i;
	int		j;

	printf("USING B_IN EXPORT CMD\n");
	i = 0;
	j = 0;
	while ((*env)[i])
		i++;
	ret = malloc(sizeof(char *) * (i + 2));
	if (!ret)
		return (exit(0), 1);
	while (j < i)
	{
		ret[j] = (*env)[j];
		j++;
	}
	ret[i] = ft_strdup(cmd[1]); //SEGV WHEN CALLING EXPORT ALONE
	if (!ret[i])
		return (0);
	i++;
	ret[i] = NULL;
	free(*env);
	*env = ret;
	return (1);
}

static int	unset_2(char ***env, int i, char **ret)
{
	if (!(*env)[i])
	{
		free(ret);
		return (1);
	}
	i++;
	free((*env)[i -1]);
	while ((*env)[i])
	{
		ret[i - 1] = (*env)[i];
		i++;
	}
	ret[i - 1] = NULL;
	free(*env);
	*env = ret;
	return (1);
}

int	ft_unset(char ***env, char **cmd)
{
	int		i;
	char	**ret;

	i = 0;
	while ((*env)[i])
		i++;
	ret = malloc(sizeof(char *) * i);
	if (!ret)
		return (exit(0), 1);
	i = 0;
	while (cmp((*env)[i], cmd[1]) && (*env)[i])
	{
		ret[i] = (*env)[i];
		i++;
	}
	return (unset_2(env, i, ret));
}
