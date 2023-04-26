/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 14:35:07 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**make_strmatrix(t_edeque *envp)
{
	int		idx;
	char	**string_arr;
	t_env	*tmp;

	idx = 0;
	string_arr = malloc(sizeof(char *) * (envp->cnt + 1));
	tmp = envp->head;
	while (tmp)
	{
		string_arr[idx] = ft_strdup(tmp->key);
		tmp = tmp->next;
		idx++;
	}
	string_arr[idx] = 0;
	return (string_arr);
}

void	sorting_strs(t_edeque *envp)
{
	int		idx;
	char	*tmp;
	char	**sorting_arr;

	idx = 0;
	sorting_arr = make_strmatrix(envp);

	idx = 0;
	// while (sorting_arr[idx])
	// {
	// 	printf("sorting_arr[%d] : %s\n", idx, sorting_arr[idx]);
	// 	idx++;
	// }
	tmp = NULL;
	while (sorting_arr[idx + 1])
	{
		printf("tmp : %p, sorting_arr[idx] : %p, sorting_arr[idx+1] : %p\n", tmp, sorting_arr[idx], sorting_arr[idx+1]);
		if (ft_strcmp(sorting_arr[idx], sorting_arr[idx + 1]) < 0)
		{
			// printf("tmp : %p, sorting_arr[idx] : %p, sorting_arr[idx+1] : %p\n", tmp, sorting_arr[idx], sorting_arr[idx+1]);
			tmp = sorting_arr[idx];
			sorting_arr[idx] = NULL;
			sorting_arr[idx] = sorting_arr[idx + 1];
			sorting_arr[idx + 1] = NULL;
			sorting_arr[idx + 1] = tmp;
			// printf("tmp : %p, sorting_arr[idx] : %p, sorting_arr[idx+1] : %p\n", tmp, sorting_arr[idx], sorting_arr[idx+1]);
			tmp = NULL;
			break ;
		}
		idx++;
	}

	idx = 0;
	while (sorting_arr[idx])
	{
		printf("sorting_arr[%d] : %s\n", idx, sorting_arr[idx]);
		idx++;
	}
}

t_edeque	*sorting_envp(t_edeque *envp)
{
	char		*envp_str;
	char		**env;
	t_edeque	*sorting_edeque;

	sorting_edeque = NULL;
	envp_str = NULL;
	env = malloc(sizeof(char *) * (envp->cnt + 1));
	return (sorting_edeque);
}

void	print_exportenvlist(t_token *token)
{
	// char		*str;
	// char		*value;
	// t_env		*tmp;
	// t_edeque	*tmp_edeque;

	// //TODO - 순서대로 출력하기 위해 추후 해시테이블 또는 이중배열 구현
	// str = NULL;
	// value = NULL;
	// tmp_edeque = sorting_envp(token->envp);
	// tmp = token->envp->head;
	// while (tmp)
	// {
	// 	ft_putstr_fd("declare -x ", 1);
	// 	value = ft_strjoin_three("\"", tmp->val, "\"");
	// 	str = ft_strjoin_three(tmp->key, "=", value);
	// 	ft_putendl_fd(str, 1);
	// 	free(value);
	// 	free(str);
	// 	tmp = tmp->next;
	// }
	sorting_strs(token->envp);
}

int	exec_export(t_token *token)
{
	if (exist_args(token))
	{
		
	}
	else
		print_exportenvlist(token);
	return (1);
}
