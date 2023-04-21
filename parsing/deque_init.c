/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:32:42 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/21 22:51:46 by hyobicho         ###   ########.fr       */
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
		printf("com[%d] %p  prev : %p, next : %p\n", i, tmp, tmp->prev, tmp->next);
		for (int n = 0; tmp->command[n]; n++)
		{
			printf("command[%d] : %s\n", n, tmp->command[n]);
		}
		i++;
		tmp = tmp->next;
	}
}