/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 17:03:55 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/08 22:18:18 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;
	size_t	idx;
	size_t	size;

	idx = (size_t)start;
	s_len = ft_strlen(s);
	size = len;
	if (idx >= s_len)
		size = 0;
	if (s_len - idx < size)
		size = s_len - idx;
	sub = (char *)malloc(sizeof(char) * (size + 1));
	if (sub == NULL)
		return (NULL);
	idx = 0;
	while (idx < size)
		sub[idx++] = s[start++];
	sub[idx] = '\0';
	return (sub);
}
