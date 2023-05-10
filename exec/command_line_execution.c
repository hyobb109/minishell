/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_execution.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:51:49 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/10 15:58:52 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	no_slash(t_token *line, char **env)
{
	struct stat	filestat;

	execve(line->command[0], line->command, env);
	stat(line->command[0], &filestat);
	if (S_ISDIR(filestat.st_mode))
	{
		print_error(line, "is a directory", 126);
	}
	else
	{
		if (access(line->command[0], F_OK | X_OK) != 0)
		{
			print_error(line, strerror(errno), 127);
		}
	}
	print_error(line, strerror(errno), 1);
}

void	no_dot(t_token *line, char **env)
{
	struct stat	filestat;

	if (ft_strlen(line->command[0]) == 1)
		print_error(line, "filename argument required", 2);
	else if (ft_strchr_idx(line->command[0], '/') < 0)
	{
		execve(line->command[0], line->command, env);
		print_error(line, "command not found", 127);
	}
	stat(line->command[0], &filestat);
	if (S_ISDIR(filestat.st_mode))
		print_error(line, "is a directory", 126);
	else
	{
		if (access(line->command[0], X_OK) != 0)
			print_error(line, strerror(errno), 127);
		if (access(line->command[0], F_OK) == 0)
			exit(0);
	}
}

void	find_command_path(t_token *line, char *path_env, char **env)
{
	char	**path;
	char	*current_path;
	char	*part_path;
	int		i;

	path = ft_split(path_env, ':');
	i = 0;
	while (path[i] != 0)
	{
		part_path = ft_strjoin(path[i], "/");
		current_path = ft_strjoin(part_path, line->command[0]);
		if (!access(current_path, F_OK))
			execve(current_path, line->command, env);
		free(current_path);
		free(part_path);
		part_path = NULL;
		current_path = NULL;
		++i;
	}
}

void	no_directory(t_token *line, char **env)
{
	if (ft_strchr_idx(line->command[0], '/') >= 0)
	{
		if (access(line->command[0], F_OK | X_OK) != 0)
		{
			print_error(line, strerror(errno), 127);
		}
	}
	execve(line->command[0], line->command, env);
	if (!is_heredoc(line->files))
	{
		print_error(line, "command not found", 127);
	}
}

void	execute_line(t_token *line, char **env)
{
	char		*path_env;
	struct stat	filestat;
	int			target_idx;

	path_env = ft_getenv(line->envp, "PATH");
	target_idx = ft_strchr_idx(line->command[0], '/');
	if (path_env == NULL)
		print_error(line, "No such file or directory", 127);
	if (!target_idx)
		no_slash(line, env);
	else
	{
		target_idx = ft_strchr_idx(line->command[0], '.');
		if (!target_idx)
			no_dot(line, env);
		else
			find_command_path(line, path_env, env);
	}
	stat(line->command[0], &filestat);
	if (S_ISDIR(filestat.st_mode) && ft_strchr(line->command[0], '/'))
		print_error(line, "is a directory", 126);
	else
		no_directory(line, env);
	print_error(line, "command not found", 127);
	exit(EXIT_FAILURE);
}
