/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:36:38 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/27 18:59:45 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exist_args(t_token *token)
{
	if (!token->command[1])
		return (0);
	return (1);
}

/* cd, export에서 사용 */
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

/* cd, */
char	*ft_getenv(t_edeque *envp, char *key)
{
	t_env	*tmp;
	char	*value;

	tmp = find_value(envp, key);
	value = tmp->val;
	return (value);
}

void	change_env(t_token *token, char *dest)
{
	char	*tmp;
	t_env	*change_env;

	change_env = find_value(token->envp, "PWD");
	tmp = change_env->val;
	free(tmp);
	change_env->val = NULL;
	change_env->val = ft_strdup(dest);
}