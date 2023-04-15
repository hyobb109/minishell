/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_parents.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:33:30 by yunjcho           #+#    #+#             */
/*   Updated: 2023/03/10 22:14:01 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	make_pipefork(t_deque *com_deque, t_file *file)
{
	int		len;
	pid_t	pid;

	len = com_deque->cnt - 2;
	file->cur_com = com_deque->head;
	file->pre_fds[READ] = -1;
	file->pre_fds[WRITE] = -1;
	while (len)
	{
		parents_proc(file, &pid);
		if (pid == 0)
			child_proc(file);
		prefds_proc(file);
		if (file->cur_com->prev == NULL)
			file->cur_com = file->cur_com->next->next;
		len--;
	}
	close(file->new_fds[READ]);
	close(file->new_fds[WRITE]);
	wait_processes(com_deque->cnt - 2);
}

void	parents_proc(t_file *file, pid_t *pid)
{
	if (pipe(file->new_fds) == -1)
		perror("Pipe Fail\n");
	*pid = fork();
}

void	prefds_proc(t_file *file)
{
	if (file->pre_fds[READ] != -1)
		close(file->pre_fds[READ]);
	if (file->pre_fds[WRITE] != -1)
		close(file->pre_fds[WRITE]);
	file->pre_fds[READ] = file->new_fds[READ];
	file->pre_fds[WRITE] = file->new_fds[WRITE];
}

void	wait_processes(int len)
{
	int	ret;
	int	count;
	int	status;

	count = 0;
	while (count < len)
	{
		ret = waitpid(-1, &status, 0);
		if (ret == -1)
			exit(EXIT_FAILURE);
		else if (WIFSIGNALED(status))
			write(1, "\n", 1);
		count++;
	}
}
