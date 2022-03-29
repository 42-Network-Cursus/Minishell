/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:54:46 by mtournay          #+#    #+#             */
/*   Updated: 2022/03/29 16:29:49 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"
#include "../../inc/builtins.h"

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
	write(1, mess, ft_strlen(mess));
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

void	nl_convert(char **s)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * ft_strlen(*s) + 1);
	if (!ret)
		return (exit(0));
	while ((*s)[i])
	{
		ret[j] = (*s)[i];
		if ((*s)[i] == '\\' && (*s)[i + 1] == 'n')
		{
			ret[j] = '\n';
			i++;
		}
		i++;
		j++;
	}
	ret[j] = '\0';
	if (i != j)
		return (free(ret));
	*s = ret;
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
