/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 17:19:34 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*find_value(t_edeque *envp, char *key)
{
	t_env	*tmp;

	tmp = envp->head;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	print_export(t_env	*print_env)
{
	char	*str;
	char	*value;

	value = ft_strjoin_three("\"", print_env->val, "\"");
	str = ft_strjoin_three(print_env->key, "=", value);
	ft_putstr_fd("declare -x ", 1); //TODO - 추후 env와 구분하는 플래그 필요하면 수정
	ft_putendl_fd(str, 1);
	free(value);
	free(str);
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

int	exec_export(t_token *token)
{
	char	**tmp;
	t_env	*env_node;

	if (exist_args(token))
	{
		env_node = malloc(sizeof(t_env));
		if (!env_node)
			ft_error();
		tmp = ft_split(token->command[1], '=');
		env_node->key = ft_strdup(tmp[0]);
		env_node->val = ft_strdup(tmp[1]);
		free_strs(tmp);
		append_back_env(token->envp, env_node);
	}
	else
		print_exportlist(token);
	return (1);
}
