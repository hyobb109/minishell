/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_deque.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 14:45:34 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/04 17:46:11 by hyobicho         ###   ########.fr       */
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

// void	append_front_env(t_edeque *deque, char *command)
// {
// 	t_env	*new_element;

// 	new_element = (t_env *)malloc(sizeof(t_env));
// 	if (!new_element)
// 		return ;
// 	init_element(new_element, command);
// 	if (!deque->head)
// 	{
// 		deque->head = new_element;
// 		deque->tail = new_element;
// 	}
// 	else
// 	{
// 		deque->head->prev = new_element;
// 		new_element->next = deque->head;
// 		deque->head = new_element;
// 	}
// 	deque->cnt++;
// }

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

t_env	*pop_select_env(t_edeque *deque, char *key)
{
	t_env	*tmp;
	t_env	*backup_prev;
	t_env	*backup_next;

	tmp = NULL;
	backup_prev = NULL;
	backup_next = NULL;
	if (deque->head == NULL || !deque || key == NULL)
		return (NULL);
	if (deque->cnt)
	{
		tmp =  find_value(deque, key);
		if (!tmp)
			return (NULL);
		else if (tmp == deque->head)
			return (pop_front_env(deque));
		else if (tmp == deque->tail)
			return (pop_back_env(deque));
		else
		{
			backup_prev = tmp->prev;
			backup_next = tmp->next;
			tmp->prev->next = backup_next;
			tmp->next->prev = backup_prev;
			tmp->prev = NULL;
			tmp->next = NULL;
			deque->cnt--;
		}
		return (tmp);
	}
	return (NULL);
}
