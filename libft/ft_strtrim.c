/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 19:45:08 by hyobicho          #+#    #+#             */
/*   Updated: 2022/11/16 21:16:00 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	incharset(char a, char const *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (a == charset[i])
			return (1);
		i++;
	}
	return (0);
}

static size_t	check_size(char const *s1, char const *set)
{
	size_t	last;
	size_t	s_len;

	s_len = ft_strlen(s1);
	if (s_len == 0)
		return (0);
	if (s_len == 1)
		return (1);
	last = s_len - 1;
	while (incharset(s1[last], set))
		last--;
	return (last + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	size_t	size;
	size_t	i;

	while (incharset(*s1, set) && *s1)
		s1++;
	size = check_size(s1, set);
	res = (char *)malloc(sizeof(char) * size + 1);
	if (res == NULL)
		return (0);
	i = 0;
	while (i < size)
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
