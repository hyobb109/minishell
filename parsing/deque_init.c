/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seulee2 <seulee2@42seoul.student.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:32:42 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/24 20:04:24 by seulee2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_deque(t_deque *deque)
{
	deque->head = NULL;
	deque->tail = NULL;
	deque->cnt = 0;
}

void	free_deque(t_deque *deque)
{
	t_token	*popped;

	popped = NULL;
	while (deque->cnt)
	{
		popped = pop_back(deque);
		free(popped);
	}
}

void	print_deque(t_deque *deque)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = deque->head;
	while (tmp)
	{
		// printf("token[%d] %p env : %p, prev : %p, next : %p\n", i, tmp, tmp->env, tmp->prev, tmp->next);
		for (int n = 0; tmp->command[n]; n++)
		{
			printf("token[%d]'s command[%d] : %s\n", i, n, tmp->command[n]);
		}
		i++;
		tmp = tmp->next;
	}
}