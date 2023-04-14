/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 20:14:44 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/14 22:21:28 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	val;
	unsigned char	*s;
	size_t			i;

	val = (unsigned char)c;
	s = (unsigned char *)b;
	i = 0;
	while (i < len)
		s[i++] = val;
	return (b);
}
