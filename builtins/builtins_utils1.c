/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:36:38 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 22:29:07 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exist_args(t_token *token)
{
	if (!token->command[1])
		return (0);
	return (1);
}

char	*swapfree_strs(char *result, char *result2)
{
	free(result);
	result = result2;
	result2 = NULL;
	return (result);
}

int	is_validkey(char *key)
{
	int	idx;

	idx = 0;
	while (key[idx])
	{
		if (!ft_isalnum(key[idx]) && key[idx] != '=' && key[idx] != '_')
			return (0);
		else if (idx == 0 && ft_isdigit(key[idx]))
			return (0);
		idx++;
	}
	return (1);
}
