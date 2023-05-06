/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edeque_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:44:41 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/06 18:30:53 by yunjcho          ###   ########.fr       */
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
	t_env	*oldpwd;
	char	**tmp;

	i = 0;
	oldpwd = NULL;
	init_edeque(envp);
	if (!env[0])
	{
		return ;
	}
	// printf("env[%d]: %s\n", 0, env[0]);
	while (env[i])
	{
		// printf("env[%d]: %s\n", i, env[i]);
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
	// printf("env[0] key: %s, value: %s\n", envp->head->key, envp->head->val);
	// for (int j = 0; env[j]; j++)
	// 	printf("env[%d]: %s\n", j, env[j]);
	// printf("env[%d]: %s\n", 1, env[1]);
}

// delete!!
void	print_edeque(t_edeque *deque)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = deque->head;
	while (tmp)
	{
		printf("env[%d] %p key: %s, value: %s, prev : %p, next : %p\n", i, tmp, tmp->key, tmp->val, tmp->prev, tmp->next);
		i++;
		tmp = tmp->next;
	}
}
