/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 15:55:25 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/22 16:32:11 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_all(char **strs, int idx)
{
    char    *home;
	char	*result;

    home = getenv("HOME");
	result = ft_strdup("\0");
	while (strs[idx])
	{
        if (idx == 1 && !ft_strcmp(strs[1], "~"))
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
	int	idx;

	idx = 0;
	if (!arguments[1])
		return (1);
	if (!ft_strncmp(arguments[1], "-n", 2))
	{
		idx = 2;
		while (arguments[1][idx])
		{
			if (arguments[1][idx] != 'n')
				return (1);
			idx++;
		}
	}
	else
		return (1);
	return (2);
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
	print_args(token->command, target_idx);
	return (1);
}