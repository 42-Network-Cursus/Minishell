/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:04:29 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 19:06:06 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "exec.h"

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
