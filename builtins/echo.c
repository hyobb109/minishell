/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 15:55:25 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/01 19:06:33 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_all(t_token *token, int idx)
{
	char	*home;
	char	*result;

	home = ft_getenv(token->envp, "HOME");
	result = ft_strdup("");
	while (token->command[idx])
	{
		if (idx == 1 && !ft_strcmp(token->command[1], "~")) //-n옵션이 무효한 경우
			result = ft_strjoin(result, home);
		else if (idx >= 2 && !ft_strcmp(token->command[2], "~")) //-n 옵션이 유효한 경우
			result = ft_strjoin(result, home);
		else
			result = ft_strjoin(result, token->command[idx]);
		if (token->command[idx + 1])
			result = ft_strjoin(result, "");
		idx++;
	}
	return (result);
}

// echo 명령어에서 출력을 시작할 행의 인덱스를 반환
int	check_option(char **arguments)
{
	int	idx1;
	int	idx2;

	idx1 = 1;
	idx2 = 0;
	if (!arguments[1])
		return (idx1);
	while (arguments[idx1])
	{
		idx2 = 0;
		if (!ft_strncmp(arguments[idx1], "-n", 2))
		{
			idx2 = 2;
			while (arguments[idx1][idx2])
			{
				if (arguments[idx1][idx2] != 'n')
					return (idx1);
				idx2++;
			}
		}
		else
			return (idx1);
		idx1++;
	}
	return (idx1);
}

void	print_args(t_token *token, int target_idx)
{
	char	*tmp;

	tmp = join_all(token, target_idx);
	if (target_idx == 1)
		printf("%s\n", tmp);
	else
		printf("%s", tmp);
	free(tmp);
}

int	exec_echo(t_token *token)
{
	int	target_idx;

	target_idx = check_option(token->command);
	print_args(token, target_idx);
	return (1);
}
