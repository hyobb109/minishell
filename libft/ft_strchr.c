/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 19:43:51 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/17 16:18:38 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	c1;
	ssize_t	i;

	c1 = (char)c;
	i = 0;
	while (s[i])
	{
		if (c1 == s[i])
			return ((char *)(s + i));
		i++;
	}
	if (c1 == '\0')
		return ((char *)(s + i));
	return (0);
}
