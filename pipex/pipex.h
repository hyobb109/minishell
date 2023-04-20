/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 22:10:59 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/20 18:19:05 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

#include "../libft/libft.h"
#include "deque.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define READ 0
#define WRITE 1

typedef struct s_file
{
	char *filepath;
	char **env;
	int pre_fds[2];
	int new_fds[2];
	t_token *cur_com;
} t_file;

void	make_comdeque(t_deque *com_deque, int ac, char **av);
void	split_comopt(t_deque *com_deque, char *str, int fd);
void	make_pipefork(t_deque *com_deque, t_file *file, char **environ);
void	parents_proc(t_file *file, pid_t *pid);
void	child_proc(t_file *file);
void	firchild_proc(t_file *file, char **arr, int *open_fd);
void	secchild_proc(t_file *file, char **arr, int *open_fd);
void	prefds_proc(t_file *file);
void	wait_processes(int len);
void	find_execpath(t_file *file, char **arr);
char	*matching_path(char **path_str, char **arr);
int		exec_builtins(t_token *token);

#endif