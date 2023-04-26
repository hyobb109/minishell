/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:38:07 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 15:29:11 by yunjcho          ###   ########.fr       */
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
	free(value);
	ft_putstr_fd("declare -x ", 1);
	ft_putendl_fd(str, 1);
}

void	print_exportenvlist(t_token *token)
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
