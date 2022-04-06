/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:06:35 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 19:06:45 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

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
		if (!cmp(env[i], str, len + 1))
			return (ft_substr_2(env[i], len + 1,
					ft_strlen_2(env[i]) - (len + 1)));
		i++;
	}
	return (NULL);
}

static size_t	n_len(int n)
{
	size_t	i;

	i = 0;
	if (n < 0)
		i++;
	while (n)
	{
		n /= 10;
		i++;
	}
	if (i == 0)
		i++;
	return (i);
}

static int	check_ges(int n)
{
	if (n == 256)
		n = 1;
	return (n);
}

char	*ft_itoa2(int n)
{
	size_t	i;
	char	*str;

	n = check_ges(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	i = n_len(n);
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		exit(1);
	if (n == 0)
		str[0] = '0';
	str[i--] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (n)
	{
		str[i] = n % 10 + 48;
		n /= 10;
		i--;
	}
	return (g_es = 0, str);
}
