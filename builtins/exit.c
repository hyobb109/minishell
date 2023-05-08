/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:37:44 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 20:31:49 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_exit(t_token *token)
{
	int	cnt;
	int	num;
	int	str_flag;

	cnt = 0;
	while (token->command[cnt])
		cnt++;
	printf("exit\n");
	str_flag = FALSE;
	if (exist_args(token))
	{
		num = ft_atoi(token->command[1], 0, 1, &str_flag);
		if (num < 0)
			g_exit_status = (256 + num % 256) * 256;
		else if (num >= 0)
			g_exit_status = (num % 256) * 256;
		if (str_flag == FALSE && cnt > 2)
		{
			ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
			printf("minishell: %s: too many arguments\n", token->command[0]);
			g_exit_status = 256;
			return ;
		}
	}
	exit(WEXITSTATUS(g_exit_status));
}
