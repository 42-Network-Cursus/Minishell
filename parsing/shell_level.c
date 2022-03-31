/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 15:20:16 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 16:41:11 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

static int	tab_len(char **tab)
{
	int	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}



int	shell_level(char ***env, char *cmd)
{
	int	i;

	i = 0;
	if (ft_strnstr(cmd, "./minishell", ft_strlen(cmd)))
	{
		if (!access(cmd, 0))
			{
				while (ncmp((*env)[i], "SHLVL", 5) && (*env)[i])
					i++;
				if (i == tab_len(*env))
					return (1);
				else
					return (0);
			}
	}
	return (1);
}