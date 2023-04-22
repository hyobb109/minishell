/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/22 16:07:47 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(void)
{
	char	cwd_name[PATH_MAX];

	getcwd(cwd_name, sizeof(cwd_name));
	ft_putendl_fd(cwd_name, 1);
	return (1);
}

int	exec_exit(t_token *token)
{
	//TODO - 프로그램 종료시 반환값 전달 가능한지 확인 후 리턴값 없애기
	//TODO - 프로그램 종료 실패 시 예외처리 추가?
	ft_putstr_fd("exit\n", 1);
	return (!kill(token->pid, SIGKILL));
}

int	exec_builtins(t_token *token)
{
	if (!ft_strcmp(ft_strlowcase(token->command[0]), "echo"))
		return(exec_echo(token));
	else if (!ft_strcmp(ft_strlowcase(token->command[0]), "pwd"))
		return (exec_pwd());
	else if (!ft_strcmp(ft_strlowcase(token->command[0]), "env"))
		return (1);
	else if (!ft_strcmp(token->command[0], "cd"))
		return (1);
	else if (!ft_strcmp(token->command[0], "export"))
		return (1);
	else if (!ft_strcmp(token->command[0], "unset"))
		return (1);
	else if (!ft_strcmp(token->command[0], "exit"))
		return (exec_exit(token));
	return (0);
}
