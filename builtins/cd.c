/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:08:27 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 15:43:25 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

static void	free_many(char *buf, char *new_dest)
{
	free(new_dest);
	free(buf);
}

static char	*dot_dot(char *buf, char *ret)
{
	int		i;
	int		j;

	j = 0;
	i = ft_strlen(buf);
	while (buf[i] != '/')
		i--;
	if (i == 0)
		i = 1;
	ret = malloc(sizeof(char) * (i + 1));
	if (!ret)
		return (exit(0), "0");
	while (j < i)
	{
		ret[j] = buf[j];
		j++;
	}
	ret[j] = '\0';
	return (ret);
}

static int	ft_cd_2(char **cmd, char *buf, char *new_dest)
{
	if (!ft_strncmp(cmd[1], "..", 3))
	{
		new_dest = dot_dot(buf, new_dest);
		chdir(new_dest);
		free_many(buf, new_dest);
		return (1);
	}
	if (cmd[1][0] == '/')
	{
		if (chdir(cmd[1]) == -1)
			return (error_mess(cmd[0],
					": no such file or directory: ", cmd[1], 2));
		free(buf);
	}
	else
	{
		new_dest = ft_strjoin(buf, cmd[1]);
		if (!new_dest)
			return (0);
		if (chdir(cmd[1]) == -1)
			return (error_mess(cmd[0],
					": no such file or directory: ", cmd[1], 2));
		free_many(buf, new_dest);
	}
	return (1);
}

int	ft_cd(char **cmd)
{
	char	*buf;
	char	*new_dest;
	int		i;

	i = 0;
	new_dest = NULL;
	while (cmd[i])
		i++;
	if (!cmd[1])
	{
		new_dest = ft_strjoin("/Users/", getenv("USER"));
		chdir(new_dest);
		free(new_dest);
		return (1);
	}
	if (i >= 3)
		return (error_mess(cmd[0], ": string not in pwd: ", cmd[1], 1));
	get_cd(&buf);
	return (ft_cd_2(cmd, buf, new_dest));
}
