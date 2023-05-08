/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 14:15:39 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/08 14:15:52 by hyobicho         ###   ########.fr       */
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
