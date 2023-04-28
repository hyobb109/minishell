/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcha_idx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:18:53 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/28 19:19:12 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strchr_idx(char *str, char c)
{
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == c)
			return (idx);
		idx++;
	}
	return (-1);
}
