/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 14:03:02 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/05 16:21:56 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_size(char *src)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (src[i])
	{
		if (src[i] > 0)
			len++;
		i++;
	}
	return (len);
}

char	*ft_strcpy(char *dest, char *src)
{
	int		i;
	int		j;
	int		src_len;


	i = 0;
	j = 0;
	src_len = count_size(src);
	dest = (char *)malloc(sizeof(char) * (src_len + 1));
	if (!dest)
		ft_error();
	while (src[i])
	{
		if (src[i] > 0)
			dest[j++] = src[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}
