/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/20 18:47:49 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_proc(t_file *file)
{
	int		open_fd;
	char	**arr;

	open_fd = 0;
	arr = NULL;
	// printf("--------Child Start--------- : %ld\n", (long)getpid());
	// printf("file command : %s\n", file->cur_com->command);
	// printf("pre_fds : %p\nfds[0] : %d, fds[1] : %d\n", file->pre_fds, file->pre_fds[0], file->pre_fds[1]);
	// printf("new_fds : %p\nfds[0] : %d, fds[1] : %d\n", file->new_fds, file->new_fds[0], file->new_fds[1]);
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
	char	*str;
	
	str = NULL;
	if (!file->cur_com->infile)
	{
		//echo | ls -al 작동 구현 (infile 지정X)
		file->filepath = "in_tmp";
		*open_fd = open(file->filepath, \
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	else
	{
		//< infile ls -al | wc -l > outfile 작동 구현 (infile 지정O)
		file->filepath = file->cur_com->infile;
		*open_fd = open(file->filepath, O_RDONLY);
	}
	if (*open_fd == -1)
		perror(file->filepath);
	else
	{	
		dup2(*open_fd, STDIN_FILENO);
		dup2(file->new_fds[WRITE], STDOUT_FILENO);
		close(file->new_fds[READ]);
		close(file->new_fds[WRITE]);

		//TODO - 추후 간단하게 변경(parsing)
		str = ft_strjoin_three(file->cur_com->command, " ", file->cur_com->args);
		arr = ft_split(str, ' ');
		free(str);
		find_execpath(file, arr);
	}
}

void	secchild_proc(t_file *file, char **arr, int *open_fd)
{
	char	*str;
	
	str = NULL;
	if (!file->cur_com->outfile)
		file->filepath = "out_tmp";
	else
		file->filepath = file->cur_com->outfile;
	*open_fd = open(file->filepath, \
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
		
		//TODO - 추후 간단하게 변경(parsing)
		str = ft_strjoin_three(file->cur_com->command, " ", file->cur_com->args);
		arr = ft_split(str, ' ');
		free(str);
		find_execpath(file, arr);
	}
}
