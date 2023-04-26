/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_parents.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:33:30 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 17:23:17 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parents_process(t_deque *cmd_deque)
{
	int	(*fd)[2];
	int	count;

	//TODO - builtin 상의
	if (cmd_deque->cnt == 1)
	{
		int debugging = exec_builtins(cmd_deque->head);
		printf("debugging : %d\n", debugging);
		// if (debugging == 1) //TODO - 나중에 주석 풀기
		// 	exit(0);
	}
	//TODO - builtin 상의

	fd = create_pipe(cmd_deque);
	//TODO - Debbuging
	// int idx = 0;
	// while (idx < 5)
	// {
	// 	printf("fd[%d][R] : %d, fd[%d][W] : %d\n", idx, fd[idx][0], idx, fd[idx][1]);
	// 	idx++;
	// }
	count = cmd_deque->cnt - 1;
	create_child(cmd_deque, fd);
	close_pipe(fd, count);
	wait_child(count);
	free_deque(cmd_deque);
}

void	wait_child(int count)
{
	int	idx;
	
	idx = 0;
	while (idx < count)
	{
		wait(NULL);
		++idx;
	}
}

void	close_pipe(int (*fd)[2], int count)
{
	int	idx;

	idx = 0;
	while (idx < count)
	{
		close(fd[idx][0]);
		close(fd[idx][1]);
		free(fd[idx]);
		++idx;
	}
}

void	create_child(t_deque *cmd_deque, int (*fd)[2])
{
	t_token	*line;
	int		count;
	int		total;

	count = 0;
	total = cmd_deque->cnt;
	while (deque_is_empty(cmd_deque))
	{
		line = pop_front(cmd_deque);
		line->pid = fork();
		if (line->pid == -1)
			exit(1);
		else if (!line->pid)
			child_process(line, count, total, fd);
		++count;
	}
}

int (*create_pipe(t_deque *cmd_deque))[2]
{
	int (*fd)[2];
	int	idx;

	fd = malloc(sizeof(int *) * cmd_deque->cnt - 1);
	idx = 0;
	while (idx < cmd_deque->cnt - 1)
	{
		pipe(fd[idx]);
		++idx;
	}
	return (fd);
}
