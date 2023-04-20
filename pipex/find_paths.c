/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 19:10:14 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/20 19:09:23 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	find_execpath(t_file *file, char **arr)
{
	int		i;
	char	*exec_path;
	char	**path_str;

	i = 0;
	exec_path = NULL;
	while (file->env[i])
	{
		if (!ft_strncmp(file->env[i], "PATH=", 5))
		{
			path_str = ft_split(&file->env[i][5], ':');
			exec_path = matching_path(path_str, arr);
			execve(exec_path, arr, file->env);
			ft_putstr_fd("command not found: ", 2);
			ft_putendl_fd(arr[0], 2);
			break ;
		}
		i++;
	}
	ft_putstr_fd("Can't command excute : ", 2); //TODO - 터지는 케이스 bash 확인하기
	ft_putendl_fd(arr[0], 2);
	exit(EXIT_FAILURE);
}

char	*matching_path(char **path_str, char **arr)
{
	int		i;
	char	*exec_path;
	char	*command_path;

	i = 0;
	command_path = ft_strjoin("/", arr[0]);
	while (path_str[i])
	{
		exec_path = ft_strjoin(path_str[i], command_path);
		if (access(exec_path, F_OK | X_OK) == 0)
			return (exec_path);
		i++;
		free(exec_path);
	}
	free(command_path);
	return (arr[0]);
}
 