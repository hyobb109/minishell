/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/04 16:50:35 by yunjcho          ###   ########.fr       */
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
	int cnt;
	int	num;
	int	str_flag;

	cnt = 0;
	while (token->command[cnt])
		cnt++;
	printf("exit\n");
	// 인자가 여러개여도 첫번째 인자가 숫자가 아니면 exit 함
	str_flag = FALSE;
	if (exist_args(token))
	{
		num = ft_atoi(token->command[1], 0, 1, &str_flag);
		if (num < 0)
		{
			token->status = 256 + num % 256;
		}
		else if (num >= 0)
			token->status = num % 256;
		if (str_flag == FALSE && cnt > 2)
		{
			// 첫번째 인자가 str이 아니고 인자 2개 이상인 경우 에러 문구 출력하고 exit 안 함
			ft_dup2(STDERR_FILENO, STDOUT_FILENO);
			printf("minishell: %s: too many arguments\n", token->command[0]);
			token->status = 1;
			return ;
		}
		printf("status : %d\n", token->status);
	}
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
			ft_dup2(STDERR_FILENO, STDOUT_FILENO);
			printf("minishell: %s: '%s': not a valid identifier\n", token->command[0], token->command[idx]); //TODO - 에러넘버 찾기
			token->status = 1;
		}
		else
		{
			free_env = pop_select_env(token->envp, token->command[idx]);
			// printf("free_env : %p, key : %s\n", free_env, token->command[idx]);
			if (free_env)
			{
				free(free_env->key);
				if (free_env->val)
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
