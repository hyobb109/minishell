/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_execution.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:51:49 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/09 16:52:17 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_line(t_token *line, char **env)
{
	char	*path_env;
	char	**path;
	char	*current_path;
	char	*part_path;
	int		i;
	struct stat filestat;
	int 		target_idx;
	
	i = 0;
	path_env = ft_getenv(line->envp, "PATH");
	target_idx = ft_strchr_idx(line->command[0], '/');
	if (!target_idx)
	{
		execve(line->command[0], line->command, env);
		stat(line->command[0], &filestat);
		if (S_ISDIR(filestat.st_mode))
		{
			printf("#minishell: %s: is a directory\n", line->command[0]);
			exit(127);
		}
		else
		{
			if (access(line->command[0], F_OK | X_OK) != 0)
			{
				printf("##minishell: %s: %s\n", line->command[0], strerror(errno));
				exit(127);
			}
		}
		printf("~minishell: %s: %s\n", line->command[0], strerror(errno));
		exit(1);
	}
	else
	{
		target_idx = ft_strchr_idx(line->command[0], '.');
		if (!target_idx)
		{
			execve(line->command[0], line->command, env);
			stat(line->command[0], &filestat);
			if (S_ISDIR(filestat.st_mode))
			{
				printf("!!minishell: %s: is a directory\n", line->command[0]);
				exit(127);
			}
			else
			{
				if (access(line->command[0], X_OK) != 0)
				{
					printf("!!!minishell: %s: %s\n", line->command[0], strerror(errno));
					exit(127);
				}
				if (access(line->command[0], F_OK) == 0)
				{
					exit(0);
				}
			}
		}
		else
		{
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
	}
	stat(line->command[0], &filestat);
	if (S_ISDIR(filestat.st_mode))
	{
		printf(">minishell: %s: is a directory\n", line->command[0]);
		exit(127);
	}
	else
	{
		if (ft_strchr_idx(line->command[0], '/') >= 0)
		{
			if (access(line->command[0], F_OK | X_OK) != 0)
			{
				printf(">>minishell: %s: %s\n", line->command[0], strerror(errno));
				exit(127);
			}
		}
		execve(line->command[0], line->command, env);

		if (!is_heredoc(line->files))
		{
			ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
			printf("23432minishell: %s: command not found\n", line->command[0]);
			exit (127);
		}
	}
	exit(EXIT_FAILURE);
}