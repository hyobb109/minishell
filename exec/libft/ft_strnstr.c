/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 22:10:43 by hyobicho          #+#    #+#             */
/*   Updated: 2022/11/15 22:58:37 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	n_len;

	if (*needle == 0)
		return ((char *)haystack);
	i = 0;
	n_len = ft_strlen(needle);
	while (i < len && *haystack)
	{
		if (len - i < n_len)
			break ;
		if (*haystack == *needle)
		{
			if (ft_memcmp(haystack, needle, n_len) == 0)
				return ((char *)haystack);
		}
		haystack++;
		i++;
	}
	return (0);
}
