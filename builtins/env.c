/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:42:23 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/25 01:51:52 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exist_validkey(t_token *token)
{
	int		idx;
	int		flag;
	char	*tmp;

	idx = 1;
	flag = 0;
	while (token->command[idx])
	{
		tmp = ft_strlowcase(ft_strjoin("", token->command[idx]));
		if (ft_strcmp(tmp, "env"))
		{
			if (!ft_strcmp(tmp, "pwd"))
				flag = 1;
			else if (!ft_strncmp(tmp, "log", 3))
			{
				flag = 3;
				if (!ft_strcmp(tmp, "logname"))
					flag = 2;
			}
			if (flag < 3 && !token->command[idx + 1])
				return (flag);
			else
			{
				if (flag == 1)
					ft_putendl_fd("usage: pwd No need options and args", 2);
				else
					ft_putendl_fd("usage: logname", 2);
				exit (EXIT_FAILURE);// ft_error(); //TODO - 추후 확인 후 수정 (return(-1))
			}
		}
		free(tmp);
		idx++;
	}
	return (0);
}

int	exec_env(t_token *token)
{
	int	idx;
	int	flag;

	idx = 0;
	flag = 0;
	if (!exist_args(token)) //인자가 없는 경우에는 목록 출력
	{
		while (token->env[idx])
		{
			ft_putendl_fd(token->env[idx], 1);
			idx++;
		}
	}
	else
	{
		flag = exist_validkey(token);
		if (flag)
		{
			if (flag == 1)
				exec_pwd(token);
			else
				ft_putendl_fd(getenv("LOGNAME"), 1);
		}
		else
		{
			// perror(NULL);
			if (token->command[1])
				printf("env: %s: %s\n", token->command[1], strerror(ENOENT));
			exit (EXIT_FAILURE);//ft_error();
		}
	}
	return (1);
}
