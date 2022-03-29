/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 11:36:29 by lmajerus          #+#    #+#             */
/*   Updated: 2022/03/29 16:39:15 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	return ;
}

static char	**malloc_envp(char **envp, int i)
{
	char	**new;

	while (envp[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		exit(1);
	new[i] = NULL;
	i = 0;
	while (envp[i])
	{
		new[i] = ft_strdup_2(envp[i]);
		if (!new[i])
		{
			while (i--)
				free(new[i]);
			free(new);
			return (NULL);
		}
		i++;
	}
	return (new);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		ges = 130;
	}
}

static void	shell_loop(t_mini *shell, char **input)
{
	while (1)
	{
		shell->nb_cmd = 0;
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, signal_handler);
		*input = readline("Minishell 🐚$ ");
		signal(SIGQUIT, SIG_IGN);
		if ((!*input) && write(2, "\b\bexit\n", 7))
			break ;
		if (!**input)
			continue ;
		add_history(*input);
		*input = ft_strtrim_2(*input, " ");
		
		shell->nb_cmd = 0;
		shell->cmds = NULL;
		if (!parser(shell, input))
			continue ;
		free(*input);
		ft_exec_cmd(shell); //exe() deleted
		free_cmds(shell->cmds, shell->nb_cmd);
	}
	if (shell->nb_cmd > 0)
		free_cmds(shell->cmds, shell->nb_cmd);
}

int	main(int argc, char **argv, char *envp[])
{
	t_mini	shell;
	char	*input;

	(void)argc;
	(void)argv;
	ges = 0;
	input = NULL;
	shell.env = malloc_envp(envp, 0);
	shell_loop(&shell, &input);
	free_env(shell.env);
}
