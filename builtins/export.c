/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 23:10:58 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	appending(t_token *token, char *key, char *value)
{
	t_env	*env_node;

	env_node = find_value(token->envp, key);
	if (env_node)
	{
		if (value)
		{
			if (env_node->val)
				free(env_node->val);
			env_node->val = ft_strdup(value);
		}
		else if (!value && !env_node->val)
			env_node->val = NULL;
	}
	else
	{
		env_node = init_envelem(key, value);
		if (!env_node)
			return (-1);
		append_back_env(token->envp, env_node);
	}
	return (1);
}

void	make_keyvalue(t_token *token, int target_idx, int idx)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	key = ft_substr(token->command[idx], 0, target_idx);
	if ((!is_validkey(key)) || ft_strchr_idx(key, ' ') > 0)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
		printf("export: '%s': not a valid identifier\n", key);
		g_exit_status = 256;
	}
	else
	{
		if (target_idx > 0)
			value = ft_substr(token->command[idx], \
				target_idx + 1, ft_strlen(token->command[idx]));
		else
			value = NULL;
		appending(token, key, value);
	}
	free(key);
	if (value)
		free(value);
}

int	append_export(t_token *token)
{
	int		idx;
	int		target_idx;

	idx = 1;
	target_idx = -1;
	while (token->command[idx])
	{
		target_idx = ft_strchr_idx(token->command[idx], '=');
		if (!target_idx)
		{
			ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
			printf("export: '%s': not a valid identifier\n", \
				token->command[idx]);
			g_exit_status = 256;
		}
		else
			make_keyvalue(token, target_idx, idx);
		idx++;
	}
	return (1);
}

int	exec_export(t_token *token)
{
	int	result;

	result = 0;
	if (exist_args(token))
		result = append_export(token);
	else if (token->envp->cnt)
		print_exportlist(token);
	return (result);
}
