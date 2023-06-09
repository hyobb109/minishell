/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edeque_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:44:41 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 19:30:39 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_edeque(t_edeque *deque)
{
	deque->head = NULL;
	deque->tail = NULL;
	deque->cnt = 0;
}

void	free_edeque(t_edeque *deque)
{
	t_env	*popped;

	popped = NULL;
	while (deque->cnt)
	{
		popped = pop_back_env(deque);
		free(popped->key);
		free(popped->val);
		free(popped);
	}
}

void	make_envlst(t_edeque *envp, char **env)
{
	int		i;
	t_env	*env_node;
	char	**tmp;

	i = 0;
	init_edeque(envp);
	if (!env[0])
		return ;
	while (env[i])
	{
		env_node = malloc(sizeof(t_env));
		if (!env_node)
			ft_error();
		tmp = ft_split(env[i], '=');
		env_node->key = ft_strdup(tmp[0]);
		env_node->val = ft_strdup(tmp[1]);
		env_node->prev = NULL;
		env_node->next = NULL;
		free_strs(tmp);
		append_back_env(envp, env_node);
		i++;
	}
}
