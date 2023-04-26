/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sorting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:40:01 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 15:42:50 by yunjcho          ###   ########.fr       */
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

char	**sorting_strsarr(t_edeque *envp)
{
	int		idx;
	char	*tmp;
	char	**sorting_arr;

	idx = 0;
	tmp = NULL;
	sorting_arr = make_strmatrix(envp);
	while (sorting_arr[idx + 1])
	{
		if (ft_strcmp(sorting_arr[idx], sorting_arr[idx + 1]) > 0)
		{
			tmp = sorting_arr[idx];
			sorting_arr[idx] = NULL;
			sorting_arr[idx] = sorting_arr[idx + 1];
			sorting_arr[idx + 1] = NULL;
			sorting_arr[idx + 1] = tmp;
			tmp = NULL;
			idx = 0;
			continue ;
		}
		idx++;
	}
	return (sorting_arr);
}
