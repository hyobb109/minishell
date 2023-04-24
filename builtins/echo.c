/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 15:55:25 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/24 20:50:40 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_all(char **strs, int idx)
{
	char	*home;
	char	*result;

	home = getenv("HOME");
	result = ft_strdup("\0");
	while (strs[idx])
	{
		if (idx == 1 && !ft_strcmp(strs[1], "~")) //옵션이 무효한 경우
			result = ft_strjoin(result, home);
		else if (idx == 2 && !ft_strcmp(strs[2], "~")) //옵션이 유효한 경우
			result = ft_strjoin(result, home);
		else
			result = ft_strjoin(result, strs[idx]);
		if (strs[idx + 1])
			result = ft_strjoin(result, " ");
		idx++;
	}
	if (result[0] == '\0')
	{
		free(result);
		return (0);
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

void	print_args(char **arguments, int target_idx)
{
	char	*tmp;

	tmp = join_all(arguments, target_idx);
	if (target_idx == 1)
		ft_putendl_fd(tmp, 1);
	else
		ft_putstr_fd(tmp, 1);
	free(tmp);
}

int	exec_echo(t_token *token)
{
	int		target_idx;

	target_idx = check_option(token->command);
	// printf("target idx : %d\n", target_idx);
	print_args(token->command, target_idx);
	return (1);
}
