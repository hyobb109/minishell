/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:32:42 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/16 19:17:54 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

// echo 명령어에서 join 시작할 인덱스를 리턴
int	check_option(char **parsed)
{
	int	idx;

	if (!strncmp(parsed[1], "-n", 2))
	{
		idx = 2;
		while (parsed[1][idx])
		{
			if (parsed[1][idx] != 'n')
				return (1);
			idx++;
		}
		return (2);
	}
	return (1);
}

void	init_element(t_token *element, char **parsed)
{
	int	args_idx;

	args_idx = 1;
	//옵션 추후 필요하면 분리
	element->command = parsed[0];
	if (!strcmp(ft_strlowcase(parsed[0]), "echo"))
		args_idx = check_option(parsed);
	if (args_idx == 2)
		element->option = parsed[1];
	else
		element->option = NULL;
	element->args = join_all(parsed, args_idx); // option 유뮤 / opention 정상적인지;
	element->prev = NULL;
	element->next = NULL;
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
		printf("com[%d] command : %s, option : %s, args : %s, prev : %p, next : %p\n", i, tmp->command, tmp->option, tmp->args, tmp->prev, tmp->next);
		i++;
		tmp = tmp->next;
	}
}