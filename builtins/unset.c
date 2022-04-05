/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:20:41 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 16:26:42 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
