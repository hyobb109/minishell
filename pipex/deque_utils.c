/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:32:42 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/15 21:15:05 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void	init_deque(t_deque *deque)
// {
// 	deque->head = NULL;
// 	deque->tail = NULL;
// 	deque->cnt = 0;
// }

void	init_element(t_token *element, char **token)
{
	//옵션 추후 필요하면 분리
	element->command = token[0];
	element->option = NULL;
	element->others = NULL;
	element->prev = NULL;
	element->next = NULL;
}

// void	free_deque(t_deque *deque)
// {
// 	t_argv	*popped;

// 	popped = NULL;
// 	while (deque->cnt)
// 	{
// 		popped = pop_back(deque);
// 		free(popped);
// 	}
// }

// void	make_comdeque(char *token)
// {
// 	int		idx;
// 	int		lastpre_idx;

// 	idx = 1;
// 	lastpre_idx = ac - 2;
// 	while (av[idx])
// 	{
// 		append_back(com_deque, av[idx]);
// 		idx++;
// 	}
// }
