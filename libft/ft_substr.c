/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 17:03:55 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/17 14:47:12 by hyobicho         ###   ########.fr       */
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
		ft_error();
	idx = 0;
	while (idx < size)
		sub[idx++] = s[start++];
	sub[idx] = '\0';
	return (sub);
}
