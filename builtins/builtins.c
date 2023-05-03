/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/03 14:16:22 by yunjcho          ###   ########.fr       */
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
	int cnt = 0;

	while (token->command[cnt])
	{
		cnt++;
	}
	
	printf("exit\n");
	if (cnt > 2)
	{
		printf("minishell: exit: too many arguments\n");
		token->status = 1;
		return ;
	}
	else if (cnt == 2)
	{
		int num = ft_atoi(token->command[1]);
		if (num < 0)
		{
			token->status = 256 + num % 256;
		}
		else
			token->status = num % 256;
	}
	printf("status : %d\n", token->status);
	exit(0);
}

int	exec_unset(t_token *token)
{
	int		idx;
	t_env	*free_env;

	idx = 1;
	while (token->command[idx])
	{
		if (!ft_isalpha(token->command[idx][0]) && token->command[idx][0] != '_')
		{
			printf("export: '%s': not a valid identifier\n", token->command[idx]); //TODO - 에러넘버 찾기
			token->status = 1;
		}
		else
		{
			free_env = pop_select_env(token->envp, token->command[idx]);
			// printf("free_env : %p, key : %s\n", free_env, token->command[idx]);
			if (free_env)
			{
				free(free_env->key);
				free(free_env->val);
				free(free_env);
			}
			// print_edeque(token->envp);
		}
		idx++;
	}
	return (1);
}

int	exec_builtins(t_token *token) //return값이 -1이면 실패
{
	char *tmp;

	tmp = ft_strlowcase(ft_strdup(token->command[0]));
	if (!ft_strcmp(tmp, "echo"))
		return (exec_echo(token));
	else if (!ft_strcmp(tmp, "pwd"))
		return (exec_pwd(token));
	else if (!ft_strcmp(tmp, "env"))
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
