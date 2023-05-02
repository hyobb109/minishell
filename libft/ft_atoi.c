/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 04:50:31 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/02 20:44:27 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	whitespace(char a)
{
	if ((a >= 9 && a <= 13) || a == 32)
		return (1);
	return (0);
}

static	int	check_error(long long res, int sign, char num)
{
	if (sign == 1)
	{
		if (res > 922337203685477580)
			return (-1);
		if (res == 922337203685477580 && num >= '7')
			return (-1);
	}
	if (sign == -1)
	{
		if (res > 922337203685477580)
			return (0);
		if (res == 922337203685477580 && num >= '8')
			return (0);
	}
	return (res);
}

int	ft_atoi(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	sign = 1;
	i = 0;
	while (whitespace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
	{
		printf("minishell: exit: %s: numeric argument required\n", str);
		return (255);
	}
	while (ft_isdigit(str[i]))
	{
		if (res >= 922337203685477580)
		{
			res = check_error(res, sign, str[i]);
			if (res == 0 || res == -1)
			{
				printf("minishell: exit: %s: numeric argument required\n", str);
				return (255);
			}
		}
		res = res * 10 + str[i] - '0';
		i++;
	}
	if (str[i] !='\0')
	{
		printf("minishell: exit: %s: numeric argument required\n", str);
		return (255);
	}
	return (res * sign);
}
