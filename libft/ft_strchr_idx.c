/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_idx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:18:53 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/05 15:35:14 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strchr_idx(char *str, char c)
{
	int	idx;

	idx = 0;
	if (!str)
		return (-1);
	while (str[idx])
	{
		if (str[idx] == c)
			return (idx);
		idx++;
	}
	return (-1);
}
