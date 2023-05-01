/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/01 23:03:25 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(t_token *token)
{
	char	cwd_name[PATH_MAX];

	(void) token;
	getcwd(cwd_name, sizeof(cwd_name));
	printf("%s\n", cwd_name);
	return (1);
}

void	exec_exit(t_token *token)
{
	//TODO - 프로그램 종료시 반환값 전달 가능한지 확인 후 리턴값 없애기
	//TODO - 프로그램 종료 실패 시 예외처리 추가?
	(void) token;
	printf("exit\n");
	exit(0);
}

int	exec_unset(t_token *token)
{
	int		idx;
	t_env	*free_env;

	idx = 1;
	while (token->command[idx])
	{
		free_env = pop_select_env(token->envp, token->command[idx]);
		// printf("free_env : %p, key : %s\n", free_env, token->command[idx]);
		if (free_env)
			free(free_env);
		// print_edeque(token->envp);
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
		return (exec_export(token));
	else if (!ft_strcmp(token->command[0], "unset"))
		return (exec_unset(token));
	else if (!ft_strcmp(token->command[0], "exit"))
	{
		exec_exit(token);
		return (1);
	}
	return (0);
}
