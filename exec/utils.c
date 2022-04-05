/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtournay <mtournay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:14:27 by mtournay          #+#    #+#             */
/*   Updated: 2022/04/05 19:14:38 by mtournay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"

void	unlink_error(void)
{
	error_mess("Minishell:", "Unlink failed", 0, 255);
	exit(0);
}
