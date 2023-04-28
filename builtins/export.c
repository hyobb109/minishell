/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/28 19:20:54 by yunjcho          ###   ########.fr       */
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

int	append_export(t_token *token)
{
	int		idx;
	int		target_idx;
	char	*val;
	char	*tmp;
	t_env	*env_node;

	idx = 1;
	target_idx = 0;
	val = NULL;
	tmp = NULL;
	while (token->command[idx])
	{
		//TODO - 유효한 키인지 확인 #, &, *, (, ), | 는 에러
		env_node = find_value(token->envp, token->command[idx]);
		if (env_node)
		{
			target_idx = ft_strchr_idx(token->command[idx], '=');
			if (target_idx > 0)
			{
				tmp = env_node->val;
				free(tmp);
				env_node->val = ft_substr(token->command[idx], \
					target_idx + 1, ft_strlen(token->command[idx]) - 2);
			}	
		}
		idx++;
	}

	// env_node->key = ft_substr(token->command[idx], 0, target_idx - 0);


	// //TODO - 유효한 키인지 확인 #, &, *, (, ), | 는 에러
	// if (env_node)
	// {
	// 	target_idx = ft_strchr_idx(token->command[1], '=');
	// 	if (target_idx > 0)
	// 	{
			
	// 	}
	// 	else
		
	// }
	// if (!find_value(token->envp, token->command[1]))
	// {
	// 	env_node = malloc(sizeof(t_env));
	// 	if (!env_node)
	// 		ft_error();
	// 	tmp = ft_strdup(ft_substr(token->command[1], 0, target_idx - 1));
	// 	env_node->key = tmp;
	// 	free(tmp);
	// 	tmp = ftstrdup(ft_substr(token->command[1], target_idx++, ft_strlen(token->command[1])));
	// 	if (tmp)
	// 		env_node->val = ft_strdup(tmp);
	// 	else
	// 		env_node->val = NULL;
	// 	free(tmp);
	// 	env_node->prev = NULL;
	// 	env_node->next = NULL;
	// 	// free_strs(tmp);
	// }
	// else
	// tmp = ft_split(token->command[1], '=');
	// env_node->key = ft_strdup(tmp[0]);
	// if (tmp[1])
	// 	env_node->val = ft_strdup(tmp[1]);
	// else
	// 	env_node->val = NULL;
	// env_node->prev = NULL;
	// env_node->next = NULL;
	// free_strs(tmp);
	append_back_env(token->envp, env_node);
	return (1);
}

int	exec_export(t_token *token)
{
	if (exist_args(token))
		append_export(token);
	else
		print_exportlist(token);
	return (1);
}
