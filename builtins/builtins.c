/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/24 20:43:45 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(t_token *token)
{
	char	cwd_name[PATH_MAX];

	(void) token;
	getcwd(cwd_name, sizeof(cwd_name));
	ft_putendl_fd(cwd_name, 1);
	return (1);
}

void	exec_exit(t_token *token)
{
	//TODO - 프로그램 종료시 반환값 전달 가능한지 확인 후 리턴값 없애기
	//TODO - 프로그램 종료 실패 시 예외처리 추가?
	(void) token;
	ft_putstr_fd("exit\n", 1);
	exit(0);
}

int	exist_args(t_token *token)
{
	int	idx;

	idx = 0;
	if (!token->command[1])
		return (0);
	return (1);
}

int	exec_cd(t_token *token)
{
	int	idx;

	idx = 0;
	while (token->env[idx])
	{
		ft_putendl_fd(token->env[idx], 1);
		idx++;
	}
	return (1);
}

int	exec_builtins(t_token *token)
{
	if (!ft_strcmp(ft_strlowcase(token->command[0]), "echo"))
		return (exec_echo(token));
	else if (!ft_strcmp(ft_strlowcase(token->command[0]), "pwd"))
		return (exec_pwd(token));
	else if (!ft_strcmp(ft_strlowcase(token->command[0]), "env"))
		return (exec_env(token));
	else if (!ft_strcmp(token->command[0], "cd"))
		return (exec_cd(token));
	else if (!ft_strcmp(token->command[0], "export"))
		return (1);
	else if (!ft_strcmp(token->command[0], "unset"))
		return (1);
	else if (!ft_strcmp(token->command[0], "exit"))
	{
		exec_exit(token);
		return (1);
	}
	return (0);
}
