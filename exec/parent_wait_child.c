/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_wait_child.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:37:37 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/10 16:15:20 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_child(int count, t_deque *cmd_deque)
{
	int		idx;
	int		status;
	pid_t	pid;

	idx = 0;
	while (idx < count + 1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			exit(EXIT_FAILURE);
		find_child(cmd_deque, status, pid);
		++idx;
	}
	ft_signal_set();
	g_exit_status = cmd_deque->tail->status;
}

void	find_child(t_deque *cmd_deque, int status, pid_t pid)
{
	t_token	*cur_point;

	cur_point = cmd_deque->head;
	while (cur_point != NULL)
	{
		if (cur_point->pid == pid)
		{
			if (WIFSIGNALED(status))
			{
				if (status == SIGQUIT)
					printf("Quit\n");
				else
					printf("\n");
				cur_point->status = (128 + status) * 256;
			}
			else
				cur_point->status = status;
		}
		cur_point = cur_point->next;
	}
}
