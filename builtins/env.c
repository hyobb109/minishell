/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:42:23 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/25 15:31:12 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_validkeyflag(t_token *token, int idx, char *tmp, int *flag)
{
	if (!ft_strcmp(tmp, "pwd"))
		*flag = 1;
	else if (!ft_strncmp(tmp, "log", 3))
	{
		*flag = 3;
		if (!ft_strcmp(tmp, "logname"))
			*flag = 2;
	}
	if (*flag < 3 && !token->command[idx + 1])
		return (*flag);
	else
	{
		if (*flag == 1)
			ft_putendl_fd("usage: pwd No need options and args", 2);
		else
			ft_putendl_fd("usage: logname", 2);
		exit (EXIT_FAILURE);//TODO - 추후 확인 후 수정 (return(-1))
	}
	return (*flag);
}

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
			if (init_validkeyflag(token, idx, tmp, &flag) > 0)
			{
				free(tmp);
				return (flag);
			}
		}
		free(tmp);
		idx++;
	}
	return (0);
}

void	print_invalidargserror(t_token *token)
{
	int		idx;
	int		print_idx;
	char	*msg_str;

	idx = 1;
	print_idx = 1;
	while (token->command[idx])
	{
		msg_str = ft_strlowcase(ft_strjoin("", token->command[idx]));
		if (ft_strcmp(msg_str, "env"))
			break ;
		free(msg_str);
		idx++;
		print_idx++;
	}
	printf("%s: %s: %s\n", token->command[print_idx - 1], \
	token->command[print_idx], strerror(ENOENT));
	exit (EXIT_FAILURE); //ft_error();
}

void	print_envlist(t_token *token)
{
	int	idx;

	idx = 0;
	while (token->env[idx])
	{
		ft_putendl_fd(token->env[idx], 1);
		idx++;
	}
}

int	exec_env(t_token *token)
{
	int	flag;

	flag = 0;
	if (!exist_args(token)) //인자가 없는 경우에는 목록 출력
		print_envlist(token);
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
			print_invalidargserror(token);
	}
	return (1);
}
