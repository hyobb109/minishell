/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:41:13 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 19:34:21 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dest;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s1);
	dest = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!dest)
		ft_error();
	if (!s1)
	{
		dest[0] = '\0';
		return (dest);
	}
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
