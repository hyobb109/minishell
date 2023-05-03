/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:02:31 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/17 16:17:11 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	c1;
	ssize_t	i;

	c1 = (char)c;
	i = 0;
	while (s[i])
		i++;
	if (c1 == '\0')
		return ((char *)(s + i));
	while (--i >= 0)
	{
		if (c1 == s[i])
			return ((char *)(s + i));
	}
	return (0);
}
