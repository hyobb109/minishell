/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:32:42 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/21 16:26:52 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_deque(t_deque *deque)
{
	deque->head = NULL;
	deque->tail = NULL;
	deque->cnt = 0;
}

char	*join_all(char **strs, int idx)
{
	char	*result;

	result = ft_strdup("\0");
	while (strs[idx])
	{
		result = ft_strjoin(result, strs[idx]);
		if (strs[idx + 1])
			result = ft_strjoin(result, " ");
		idx++;
	}

	if (result[0] == '\0')
	{
		free(result);
		return (0);
	}
	return (result);
}

void	init_element(t_token *element, char **parsed)
{
	int	args_idx;

	args_idx = 1;
	//옵션 추후 필요하면 분리
	element->command = parsed[0];
	element->args = join_all(parsed, args_idx); // option 유뮤 / opention 정상적인지;
	element->prev = NULL;
	element->next = NULL;

	//TODO - infile/outfile
	element->infile = NULL;
	element->outfile = NULL;
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

void	print_deque(t_deque *deque)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = deque->head;
	while (tmp)
	{
		printf("com[%d] %p command : %s, args : %s, prev : %p, next : %p\n", i, tmp, tmp->command, tmp->args, tmp->prev, tmp->next);
		i++;
		tmp = tmp->next;
	}
}