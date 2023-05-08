/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 15:55:25 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 17:59:49 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_all(t_token *token, int idx)
{
	char	*result;
	char	*result2;

	result = ft_strdup("");
	result2 = NULL;
	while (token->command[idx])
	{
		if (idx == 1 && !ft_strcmp(token->command[1], "~"))
			result2 = ft_strjoin(result, ft_getenv(token->envp, "HOME"));
		else if (idx >= 2 && !ft_strcmp(token->command[2], "~"))
			result2 = ft_strjoin(result, ft_getenv(token->envp, "HOME"));
		else
			result2 = ft_strjoin(result, token->command[idx]);
		result = swapfree_strs(result, result2);
		if (token->command[idx + 1])
		{
			result2 = ft_strjoin(result, " ");
			result = swapfree_strs(result, result2);
		}
		idx++;
	}
	return (result);
}

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
