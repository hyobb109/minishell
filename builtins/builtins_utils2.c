/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 22:28:24 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/09 19:34:52 by hyunwoju         ###   ########.fr       */
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

t_env	*init_envelem(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		ft_error();
	if (key)
		new->key = ft_strdup(key);
	else
		new->key = ft_strdup("");
	if (value)
		new->val = ft_strdup(value);
	else
		new->val = NULL;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

char	*ft_getenv(t_edeque *envp, char *key)
{
	t_env	*tmp;
	char	*value;

	tmp = find_value(envp, key);
	if (tmp)
	{
		value = tmp->val;
		return (value);
	}
	return (NULL);
}
