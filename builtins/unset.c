/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 16:47:19 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 22:29:17 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_token *token, int idx)
{
	t_env	*free_env;

	free_env = pop_select_env(token->envp, token->command[idx]);
	if (free_env)
	{
		free(free_env->key);
		if (free_env->val)
			free(free_env->val);
		free(free_env);
	}
}

int	exec_unset(t_token *token)
{
	int	idx;

	idx = 1;
	while (token->command[idx])
	{
		if ((!is_validkey(token->command[idx])) || \
			ft_strchr_idx(token->command[idx], ' ') > 0)
		{
			ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
			printf("minishell: %s: '%s': not a valid identifier\n", \
				token->command[0], token->command[idx]);
			g_exit_status = 256;
		}
		else
			free_env(token, idx);
		idx++;
	}
	return (1);
}
