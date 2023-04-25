/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:23:22 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/25 21:26:45 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	append_front(t_deque *deque, char *command)
// {
// 	t_token	*new_element;

// 	new_element = (t_token *)malloc(sizeof(t_token));
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


void	append_back(t_deque *deque, t_token *token)
{
	if (!deque->tail)
	{
		deque->head = token;
		deque->tail = token;
	}
	else
	{
		token->prev = deque->tail;
		deque->tail->next = token;
		deque->tail = token;
	}
	deque->cnt++;
}

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

t_token	*pop_front(t_deque *deque)
{
	t_token	*tmp;

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

t_token	*pop_back(t_deque *deque)
{
	t_token	*tmp;

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

int	deque_is_empty(t_deque *pdeque)
{
	if (pdeque->head == NULL)
		return TRUE;
	else
		return FALSE;
}


