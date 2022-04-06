/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwastche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 18:06:09 by cwastche          #+#    #+#             */
/*   Updated: 2022/04/06 18:06:10 by cwastche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

void	realloc_env(int j, char **ret, char **env)
{
	ret[j] = ft_strdup(env[j]);
	if (!ret)
		exit(0);
	free(env[j]);
}

void	count_val(t_count *c)
{
	c->i = 0;
	c->j = -1;
}
