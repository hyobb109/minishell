/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/27 22:17:05 by yunjcho          ###   ########.fr       */
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

// int	append_export(t_token *token)
// {
// 	char	*tmp;
// 	t_env	*env_node;

// 	env_node = NULL;
// 	tmp = ft_strchr(token->command[1], '=', );
// 	printf("target idx : %d\n", target_idx);
// 	if (!find_value(token->envp, token->command[1]))
// 	{
// 		env_node = malloc(sizeof(t_env));
// 		if (!env_node)
// 			ft_error();
// 		tmp = ft_strdup(ft_substr(token->command[1], 0, target_idx - 1));
// 		env_node->key = tmp;
// 		free(tmp);
// 		tmp = ftstrdup(ft_substr(token->command[1], target_idx++, ft_strlen(token->command[1])));
// 		if (tmp)
// 			env_node->val = ft_strdup(tmp);
// 		else
// 			env_node->val = NULL;
// 		free(tmp);
// 		env_node->prev = NULL;
// 		env_node->next = NULL;
// 		// free_strs(tmp);
// 	}
// 	else
// 	tmp = ft_split(token->command[1], '=');
// 	env_node->key = ft_strdup(tmp[0]);
// 	if (tmp[1])
// 		env_node->val = ft_strdup(tmp[1]);
// 	else
// 		env_node->val = NULL;
// 	env_node->prev = NULL;
// 	env_node->next = NULL;
// 	free_strs(tmp);
// 	append_back_env(token->envp, env_node);
// }

int	exec_export(t_token *token)
{
	if (exist_args(token))
		printf("hi\n");
		// append_export(token);
	else
		print_exportlist(token);
	return (1);
}
