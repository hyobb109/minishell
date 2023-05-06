/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 12:30:29 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/06 20:46:35 by yunjcho          ###   ########.fr       */
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
	if (s2)
	{
		j = 0;
		while (s2[j])
			res[i++] = s2[j++];
	}
	res[i] = '\0';
	return (res);
}
