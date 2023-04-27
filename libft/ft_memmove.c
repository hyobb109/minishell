/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 16:52:08 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/27 20:37:42 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*dst1;
	char	*src1;
	size_t			i;

	if (dst == NULL && src == NULL)
		return (dst);
	dst1 = (char *)dst;
	src1 = (char *)src;
	if (dst < src)
	{
		i = 0;
		while (i < len)
		{
			dst1[i] = src1[i];
			i++;
		}
	}
	else
	{
		i = len;
		while (i-- > 0)
			dst1[i] = src1[i];
	}
	return (dst);
}
