/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/05 16:39:42 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(t_env	*print_env)
{
	printf("declare -x %s", print_env->key);
	if (print_env->val)
		printf("=\"%s\"", print_env->val);
	printf("\n");
}

void	print_exportlist(t_token *token)
{
	int		idx;
	char	**sorting_strs;
	t_env	*print_env;

	idx = 0;
	sorting_strs = sorting_strsarr(token->envp);
	while (sorting_strs[idx])
	{
		print_env = find_value(token->envp, sorting_strs[idx]);
		print_export(print_env);
		idx++;
	}
	free_strs(sorting_strs);
}

int	appending(t_token *token, char *key, char *value)
{
	t_env	*env_node;
	char	*free_str;

	env_node = NULL;
	free_str = NULL;
	env_node = find_value(token->envp, key);
	if (env_node)
	{
		if (!env_node->val)
		{
			if (value)
			{
				env_node->val = ft_strdup(value);
				return (1);
			}
			else
			{
				env_node->val = NULL;
				return (0);
			}
		}
		else if (env_node->val)
		{
			if (!ft_strcmp(env_node->val, value))
				return (0);
			else
			{
				free_str = env_node->val;
				free(free_str);
				env_node->val = ft_strdup(value);
			}
		}
	}
	else
	{
		env_node = malloc(sizeof(t_env));
		if (!env_node)
			ft_error();
		env_node->key = ft_strdup(key);
		if (value)
			env_node->val = ft_strdup(value);
		else
			env_node->val = NULL;
		env_node->prev = NULL;
		env_node->next = NULL;
		append_back_env(token->envp, env_node);
	}
	return (1);
}

void	append_export(t_token *token)
{
	int		idx;
	int		target_idx;
	char	*key;
	char	*value;

	idx = 1;
	target_idx = -1;
	key = NULL;
	value = NULL;
	while (token->command[idx])
	{
		target_idx = ft_strchr_idx(token->command[idx], '=');
		if (!target_idx)
		{
			ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
			printf("export: '%s': not a valid identifier\n", token->command[idx]); //TODO - 에러넘버 찾기
			token->status = 1;
		}
		else
		{
			key = ft_substr(token->command[idx], 0, target_idx);
			if ((!ft_isalpha(key[0]) && key[0] != '_') || ft_strchr_idx(key, ' ') > 0)//TODO - 유효한 키인지 확인 #, &, *, (, ), | 는 에러 / 숫자만 있어도 에러
			{
				free(key);
				ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
				printf("export: '%s': not a valid identifier\n", key); //TODO - 에러넘버 찾기
				token->status = 1;
				idx++;
				continue ;
			}
			if (target_idx > 0) // = 이 있음
				value = ft_substr(token->command[idx], \
					target_idx + 1, ft_strlen(token->command[idx]));
			else // = 이 없음, key만 있음
				value = NULL;
			if (appending(token, key, value) == -1)
			{
				free(key);
				if (value)
					free(value);
				idx++;
				continue ;
			}
			free(key);
			if (value)
				free(value);
		}
		idx++;
	}
}

int	exec_export(t_token *token)
{
	if (exist_args(token))
		append_export(token);
	else
		print_exportlist(token);
	return (1);
}
