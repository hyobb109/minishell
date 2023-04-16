/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/16 21:20:40 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_proc(t_file *file)
{
	int		open_fd;
	char	**arr;

	open_fd = 0;
	arr = NULL;
	if (!exec_builtins(file->cur_com))
	{
		if (file->cur_com->prev == NULL)
			firchild_proc(file, arr, &open_fd);
		else
			secchild_proc(file, arr, &open_fd);
	}
	close(open_fd);
	exit(EXIT_FAILURE);
}

void	firchild_proc(t_file *file, char **arr, int *open_fd)
{
	file->filepath = file->cur_com->command;
	*open_fd = open(file->filepath, O_RDONLY);
	if (*open_fd == -1)
		perror(file->cur_com->command);
	else
	{
		file->cur_com = file->cur_com->next;
		dup2(*open_fd, STDIN_FILENO);
		dup2(file->new_fds[WRITE], STDOUT_FILENO);
		close(file->new_fds[READ]);
		close(file->new_fds[WRITE]);
		arr = ft_split(file->cur_com->command, ' ');
		find_execpath(file, arr);
	}
}

void	secchild_proc(t_file *file, char **arr, int *open_fd)
{
	*open_fd = open(file->cur_com->next->command, \
	O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*open_fd == -1)
		perror(file->filepath);
	else
	{
		dup2(file->pre_fds[READ], STDIN_FILENO);
		dup2(*open_fd, STDOUT_FILENO);
		close(file->pre_fds[READ]);
		close(file->pre_fds[WRITE]);
		close(file->new_fds[READ]);
		close(file->new_fds[WRITE]);
		arr = ft_split(file->cur_com->command, ' ');
		find_execpath(file, arr);
	}
}
