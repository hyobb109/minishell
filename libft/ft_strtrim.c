/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 19:45:08 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/17 14:46:48 by hyobicho         ###   ########.fr       */
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
	size_t	j;

	i = 0;
	while (incharset(s1[i], set) && s1[i])
		i++;
	size = check_size(&s1[i], set);
	res = (char *)malloc(sizeof(char) * size + 1);
	if (res == NULL)
		ft_error();
	j = 0;
	while (j < size)
		res[j++] = s1[i++];
	res[j] = '\0';
	return (res);
}
