/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:14:27 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/06 16:43:59 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

void	unlink_error(void)
{
	error_mess("Minishell:", "Unlink failed", 0, 255);
	exit(0);
}

void	signal_handler2(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		g_es = 130;
	}
}

void	my_dup(int a, int b)
{
	int	ret;

	ret = dup2(a, b);
	if (ret == -1)
	{
		error_mess("minishell: ", "dup failed", NULL, 1);
		exit(1);
	}
}

void	close_pipe(int *end)
{
	int	ret;

	ret = close(end[0]);
	if (ret == -1)
	{
		error_mess("minishell: ", "failed to close pipe", NULL, 1);
		exit(1);
	}
	ret = close(end[1]);
	if (ret == -1)
	{
		error_mess("minishell: ", "failed to close pipe", NULL, 1);
		exit(1);
	}
}

char	*ft_cmd_path(char **env, char *cmd)
{
	int		i;
	char	**paths;

	i = 0;
	if (cmd[0] == '.' && cmd[1] == '/')
		return (cmd);
	while (!ft_strnstr(env[i], "PATH=", 5) && env[i])
		i++;
	env[i] = ft_substr(env[i], 6, ft_strlen(env[i]));
	paths = ft_split(env[i], ':');
	i = -1;
	while (paths && paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], cmd);
		if (!access(paths[i], F_OK) && !access(paths[i], X_OK))
			return (paths[i]);
		free(paths[i]);
	}
	return (0);
}
