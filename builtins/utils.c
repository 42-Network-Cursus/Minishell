/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:54:46 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/31 19:18:53 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

int	cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	error_mess(char *s, char *mess, char *cmd, int exstat)
{
	if (s)
		write(1, s, ft_strlen(s));
	if (mess)
		write(1, mess, ft_strlen(mess));
	if (cmd)
		write(1, cmd, ft_strlen(cmd));
	write(1, "\n", 1);
	ges = exstat;
	return (1);
}

int	ncmp(char *s1, char *s2, size_t n)
{
	int	i;

	i = 0;
	while ((s1[i] && s2[i]) && n > 0)
	{
		if (s1[i] != s2[i])
			break ;
		i++;
		n--;
	}
	if (n || s1[i] || s2[i])
		return (1);
	return (0);
}

char	*cat_dup(char *dst, char *src)
{
	char	*ret;
	int		i;
	int		j;

	ret = malloc(sizeof(char) * (ft_strlen(dst) + ft_strlen(src) + 2));
	if (!ret)
	{
		free(dst);
		return (NULL);
	}
	i = -1;
	j = 0;
	while (dst[++i])
		ret[i] = dst[i];
	ret[i++] = '/';
	while (src[j])
		ret[i++] = src[j++];
	ret[i] = '\0';
	free(dst);
	return (ret);
}
