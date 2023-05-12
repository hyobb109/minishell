/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 04:50:31 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/12 17:22:49 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	whitespace(char a)
{
	if ((a >= 9 && a <= 13) || a == 32)
		return (1);
	return (0);
}

static	int	check_error(long long res, int sign, char num, int *str_flag)
{
	if (sign == 1)
	{
		if (res > 922337203685477580 \
			|| (res == 922337203685477580 && num >= '7'))
		{
			if (!(res == 922337203685477580 && num == '7'))
				*str_flag = 1;
			return (-1);
		}
	}
	if (sign == -1)
	{
		if (res > 922337203685477580 \
			|| (res == 922337203685477580 && num >= '8'))
		{
			if (!(res == 922337203685477580 && num == '8'))
				*str_flag = 1;
			return (0);
		}
	}
	return (res);
}

static int	nonnumeric(int status, int *str_flag, const char *str)
{
	if (*str_flag || status == 255)
	{
		if (dup2(STDERR_FILENO, STDOUT_FILENO) < 0)
			ft_error();
		printf("minishell: exit: %s: numeric argument required\n", str);
		*str_flag = 1;
	}
	return (status);
}

static int	num_trans(const char *str, int idx, int sign, int *str_flag)
{
	long long	res;

	res = 0;
	while (ft_isdigit(str[idx]))
	{
		if (res >= 922337203685477580)
		{
			res = check_error(res, sign, str[idx], str_flag);
			if (res == 0 || res == -1)
				return (nonnumeric(res, str_flag, str));
		}
		res = res * 10 + str[idx] - '0';
		idx++;
	}
	if (str[idx] != '\0')
		return (nonnumeric(255, str_flag, str));
	return (res);
}

int	ft_atoi(const char *str, int i, int sign, int *str_flag)
{
	long long	res;

	res = 0;
	while (whitespace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (nonnumeric(255, str_flag, str));
	res = num_trans(str, i, sign, str_flag);
	if (*str_flag)
		return (res);
	return (res * sign);
}
