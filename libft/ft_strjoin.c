/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 12:30:29 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 16:43:33 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	len;
	size_t	i;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (res == NULL)
		ft_error();
	i = 0;
	j = 0;
	if (s1)
	{
		while (s1[j])
			res[i++] = s1[j++];
	}
	j = 0;
	if (s2)
	{
		while (s2[j])
			res[i++] = s2[j++];
	}
	res[i] = '\0';
	return (res);
}
