/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_printing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:50:16 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 20:50:37 by yunjcho          ###   ########seoul.kr  */
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
