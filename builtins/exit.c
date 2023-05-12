/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:37:44 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/12 17:33:34 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_exit_arg(t_token *token, int *str_flag)
{
	int	num;

	num = ft_atoi(token->command[1], 0, 1, str_flag);
	if (num < 0)
		token->status = 256 + num % 256;
	else if (num >= 0)
		token->status = num % 256;
}

void	exec_exit(t_token *token)
{
	int	cnt;
	int	str_flag;

	cnt = 0;
	while (token->command[cnt])
		cnt++;
	printf("exit\n");
	str_flag = FALSE;
	if (exist_args(token))
	{
		init_exit_arg(token, &str_flag);
		if (str_flag == FALSE && cnt > 2)
		{
			g_exit_status = 256;
			token->status = 1;
			print_error(token, "too many arguments", token->status);
			return ;
		}
		g_exit_status = token->status * 256;
	}
	exit(WEXITSTATUS(g_exit_status));
}
