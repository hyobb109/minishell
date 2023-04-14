/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 19:42:24 by hyobicho          #+#    #+#             */
/*   Updated: 2022/11/15 22:57:17 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	i;
	size_t	j;

	dst_len = ft_strlen(dst);
	j = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = 0;
	while (src[i] && j + 1 < dstsize)
	{
		dst[j] = src[i];
		j++;
		i++;
	}
	if (dstsize == 0 || dst_len > dstsize)
		return (dstsize + src_len);
	dst[j] = '\0';
	return (src_len + dst_len);
}
