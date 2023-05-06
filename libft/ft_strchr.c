/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 19:43:51 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/06 17:57:24 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	c1;
	ssize_t	i;

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
