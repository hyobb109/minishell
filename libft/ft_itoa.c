/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 21:51:14 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/17 16:44:11 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	cnt_digit(long long n)
{
	int	cnt;

	cnt = 1;
	if (n < 0)
	{
		cnt++;
		n = -n;
	}
	while (n >= 10)
	{
		cnt++;
		n = n / 10;
	}
	return (cnt);
}

static char	*putnum(char *res, size_t size, long long num, int positive)
{
	size_t	i;

	i = size;
	res[i] = '\0';
	if (num < 0)
	{
		res[0] = '-';
		num = -num;
		positive = 0;
	}
	while (--i > 0)
	{
		res[i] = num % 10 + '0';
		num = num / 10;
	}
	if (num >= 0 && positive)
		res[0] = num % 10 + '0';
	return (res);
}

char	*ft_itoa(int n)
{
	size_t		size;
	char		*res;

	size = cnt_digit((long long)n);
	res = (char *)malloc(sizeof(char) * size + 1);
	if (res == NULL)
		ft_error();
	return (putnum(res, size, (long long)n, 1));
}
