/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:54:46 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 19:02:48 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

int	cmp(char *s1, char *s2, int n)
{
	if (!s1 || !s2)
		return (1);
	while (n--)
	{
		if (*s1 != *s2 && !*s2 && *s1 == '=')
			return (0);
		if (*s1 != *s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		if (!*s1)
			return (1);
		s1++;
		s2++;
	}
	return (1);
}

int	error_mess(char *s, char *mess, char *cmd, int exstat)
{
	if (s)
		write(2, s, ft_strlen(s));
	if (mess)
		write(2, mess, ft_strlen(mess));
	if (cmd)
		write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	g_es = exstat;
	return (1);
}

int	ncmp(char *s1, char *s2, size_t n)
{
	int	i;

	i = 0;
	while ((s1[i] && s2[i]) && n >= 0)
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

void	nl_convert_process(char **s)
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
		if ((*s)[i] == '\\' && (*s)[i + 1] == '\\' && (*s)[i + 2] == 'n')
		{
			ret[j] = '\n';
			i += 2;
		}
		i++;
		j++;
	}
	ret[j] = '\0';
	if (i == j)
		return (free(ret));
	*s = ret;
}
