/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:32:42 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/04 20:45:30 by yunjcho          ###   ########.fr       */
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
		free_strs(popped->command);
		free_files(&popped->files);
		free(popped);
	}
}

void	print_deque(t_deque *deque)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = deque->head;
	// printf("token cnt: %d\n", deque->cnt);
	while (tmp)
	{
		printf("token[%d] %p prev : %p, next : %p\n", i, tmp, tmp->prev, tmp->next);
		for (int n = 0; tmp->command[n]; n++)
		{
			printf("token[%d]'s command[%d] : %s\n", i, n, tmp->command[n]);
		}
		i++;
		tmp = tmp->next;
	}
}
