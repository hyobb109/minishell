/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:25:03 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/26 22:09:51 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char	c1;
	char	*s1;
	size_t	i;

	c1 = (char)c;
	s1 = (char *)s;
	i = 0;
	while (i < n)
	{
		if (c1 == s1[i])
			return (&s1[i]);
		i++;
	}
	return (0);
}
