/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 19:53:02 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/15 21:13:56 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_strlen1(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	**result(char **res, char const *str, char c, int cnt)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (i < cnt)
	{
		len = ft_strlen1(str, c);
		res[i] = (char *)malloc(sizeof(char) * (len + 1));
		if (res[i] == NULL)
			ft_error();
		j = 0;
		while (j < len && str[j])
		{
			res[i][j] = str[j];
			j++;
		}
		res[i][j] = '\0';
		while (str[j] == c && str[j])
			j++;
		str += j;
		i++;
	}
	res[i] = 0;
	return (res);
}

static unsigned int	cntstr(char const *s, char c)
{
	int	cnt;
	int	i;
	int	flag;

	cnt = 0;
	i = 0;
	flag = 1;
	while (s[i])
	{
		if (s[i] != c && flag)
		{
			cnt++;
			flag = 0;
		}
		else if (s[i] == c)
			flag = 1;
		i++;
	}
	return (cnt);
}

char	**ft_split(char const *s, char c)
{
	char			**res;
	unsigned int	cnt;

	while (*s == c && c != '\0')
		s++;
	cnt = cntstr(s, c);
	res = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (res == NULL)
		ft_error();
	return (result(res, s, c, cnt));
}
