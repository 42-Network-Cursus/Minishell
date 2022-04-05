/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:04:11 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 19:05:58 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

char	*ft_substr_2(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	new = malloc(sizeof(*s) * (len + 1));
	if (!new)
		exit(1);
	new[len] = '\0';
	while (i < len && start + i < ft_strlen_2(s))
	{
		new[i] = s[start + i];
		i++;
	}
	return (new);
}

char	*ft_strdup_2(const char *s)
{
	size_t	s_len;
	void	*new;

	s_len = ft_strlen_2(s) + 1;
	new = malloc(s_len);
	if (!new)
		exit(1);
	return ((char *)ft_memcpy_2(new, s, s_len));
}

void	*ft_memcpy_2(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	if (dest || src)
		while (n--)
			*d++ = *s++;
	return (dest);
}

static int	ft_is_in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (42);
		i++;
	}
	return (0);
}

char	*ft_strtrim_2(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	int		i;
	char	*new;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	i = 0;
	end = ft_strlen_2(s1);
	while (s1[start] && ft_is_in_set(s1[start], set))
		start++;
	while (end > start && ft_is_in_set(s1[end - 1], set))
		end--;
	new = malloc(sizeof(*s1) * (end - start + 1));
	if (!new)
		exit(1);
	while (start < end)
		new[i++] = s1[start++];
	new[i] = '\0';
	return (new);
}
