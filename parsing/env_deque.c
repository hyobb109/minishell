/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_deque.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 14:45:34 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/09 19:36:33 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_back_env(t_edeque *deque, t_env *env)
{
	if (!deque->tail)
	{
		deque->head = env;
		deque->tail = env;
	}
	else
	{
		env->prev = deque->tail;
		deque->tail->next = env;
		deque->tail = env;
	}
	deque->cnt++;
}

t_env	*pop_front_env(t_edeque *deque)
{
	t_env	*tmp;

	if (deque->cnt)
	{
		tmp = deque->head;
		deque->head = tmp->next;
		if (deque->head)
			deque->head->prev = 0;
		tmp->next = 0;
		deque->cnt--;
		if (deque->cnt == 0)
		{
			deque->head = 0;
			deque->tail = 0;
		}
		return (tmp);
	}
	return (0);
}

t_env	*pop_back_env(t_edeque *deque)
{
	t_env	*tmp;

	if (deque->cnt)
	{
		tmp = deque->tail;
		deque->tail = tmp->prev;
		if (deque->tail)
			deque->tail->next = 0;
		tmp->prev = 0;
		deque->cnt--;
		if (deque->cnt == 0)
		{
			deque->head = 0;
			deque->tail = 0;
		}
		return (tmp);
	}
	return (0);
}

t_env	*pop_mid_env(t_env *tmp, t_edeque *deque)
{
	t_env	*backup_prev;
	t_env	*backup_next;

	backup_prev = tmp->prev;
	backup_next = tmp->next;
	tmp->prev->next = backup_next;
	tmp->next->prev = backup_prev;
	tmp->prev = NULL;
	tmp->next = NULL;
	deque->cnt--;
	return (tmp);
}

t_env	*pop_select_env(t_edeque *deque, char *key)
{
	t_env	*tmp;

	tmp = NULL;
	if (deque->head == NULL || !deque || key == NULL)
		return (NULL);
	if (deque->cnt)
	{
		tmp = find_value(deque, key);
		if (!tmp)
			return (NULL);
		else if (tmp == deque->head)
			return (pop_front_env(deque));
		else if (tmp == deque->tail)
			return (pop_back_env(deque));
		else
			return (pop_mid_env(tmp, deque));
	}
	return (NULL);
}
