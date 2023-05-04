/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 15:55:25 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/04 20:42:20 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_all(t_token *token, int idx)
{
	char	*home;
	char	*result;
	char	*result2;

	home = ft_getenv(token->envp, "HOME");
	result = ft_strdup("");
	result2 = NULL;
	while (token->command[idx])
	{
		if (idx == 1 && !ft_strcmp(token->command[1], "~")) //-n옵션이 무효한 경우
		{
			result2 = ft_strjoin(result, home);
			free(result);
			result = result2;
			result2 = NULL;
		}
		else if (idx >= 2 && !ft_strcmp(token->command[2], "~")) //-n 옵션이 유효한 경우
		{
			result2 = ft_strjoin(result, home);
			free(result);
			result = result2;
			result2 = NULL;
		}
		else
		{
			result2 = ft_strjoin(result, token->command[idx]);
			free(result);
			result = result2;
			result2 = NULL;
		}
		if (token->command[idx + 1])
		{
			result2 = ft_strjoin(result, " ");
			free(result);
			result = result2;
			result2 = NULL;
		}
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

int	exec_echo(t_token *token)
{
	int		target_idx;
	char	*result;

	target_idx = check_option(token->command);
	result = join_all(token, target_idx);
	if (target_idx == 1)
		ft_putendl_fd(result, STDOUT_FILENO);
	else
		ft_putstr_fd(result, STDOUT_FILENO);
	free(result);
	return (1);
}
