/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/05/06 18:34:21 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	c1;
	ssize_t	i;

	if (!s)
		return (0);
	c1 = (char)c;
	i = 0;
	if (!s)
		return (0);
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
