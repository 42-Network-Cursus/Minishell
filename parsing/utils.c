/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmajerus <lmajerus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 13:56:08 by lmajerus          #+#    #+#             */
/*   Updated: 2022/03/28 15:45:26 by lmajerus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	*ft_calloc_1(size_t nmemb, size_t size)
{
	void	*calloc;

	if (nmemb == 0 || size == 0)
	{
		nmemb = 1;
		size = 1;
	}
	calloc = malloc(nmemb * size);
	if (!calloc)
		exit(1);
	ft_bzero_2(calloc, nmemb * size);
	return (calloc);
}

void	ft_bzero_2(void *s, size_t n)
{
	ft_memset_2(s, 0, n);
}

void	*ft_memset_2(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = c;
	return (s);
}

t_here	*ft_lstnew_2(void *content)
{
	t_here	*new;

	new = malloc(sizeof(t_here));
	if (!new)
		exit(1);
	new->delimiter = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back_2(t_here **lst, t_here *new)
{
	if (!*lst)
		*lst = new;
	else
		ft_lstlast_2(*lst)->next = new;
}

t_here	*ft_lstlast_2(t_here *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

char	*ft_getenv(char *str, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen_2(str);
	while (env[i])
	{
		if (!ft_strncmp_2(str, env[i], len - 1))
			return (ft_substr_2(env[i], len + 1,
					ft_strlen_2(env[i]) - (len + 1)));
		i++;
	}
	return (NULL);
}
