/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 13:56:08 by lmajerus          #+#    #+#             */
/*   Updated: 2022/04/05 19:07:52 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

int	ft_strncmp_2(const char *s1, const char *s2, size_t n)
{
	while (n--)
	{
		if (*s1 != *s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		if (!*s1)
			return (0);
		s1++;
		s2++;
	}
	return (0);
}

size_t	ft_strlen_2(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void	ft_putstr_fd_2(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen_2(s));
}

int	ft_error(char *error_msg, int perror_flag)
{
	ft_putstr_fd_2("Minishell: ", 2);
	if (perror_flag)
		perror(error_msg);
	else
		ft_putstr_fd_2(error_msg, 2);
	return (1);
}

char	*ft_strjoin_2(char const *s1, char const *s2)
{
	char	*s3;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	s3 = malloc(sizeof(*s1) * (ft_strlen_2(s1) + ft_strlen_2(s2) + 1));
	if (!s3)
		exit(1);
	i = 0;
	j = 0;
	while (s1[i])
		s3[j++] = s1[i++];
	i = 0;
	while (s2[i])
		s3[j++] = s2[i++];
	s3[j] = '\0';
	return (s3);
}
