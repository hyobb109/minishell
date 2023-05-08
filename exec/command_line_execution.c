/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_execution.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:51:49 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/08 18:21:42 by hyunwoju         ###   ########.fr       */
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
	struct stat filestat2;

	path_env = ft_getenv(line->envp, "PATH");
	if (!path_env)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
		printf("minishell: %s: No such file or directory\n", line->command[0]);
		exit (127);
	}
	path = ft_split(path_env, ':');
	i = 0;
	while (path[i] != 0)
	{
		part_path = ft_strjoin(path[i], "/");
		current_path = ft_strjoin(part_path, line->command[0]);
		if (!access(current_path, F_OK))
			break ;
		free(current_path);
		current_path = NULL;
		++i;
	}
	execve(line->command[0], line->command, env); // 절대경로 로 들어왔을 때
	// 절대경로 실패
	stat(line->command[0], &filestat); // line->command[0] path 의 정보 filestat 에 저장
	stat(current_path, &filestat2);
	if (S_ISDIR(filestat.st_mode) && ft_strchr(line->command[0], '/')) // 디렉토리인지 확인 그리고 '/' 가 있다면
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
		printf("minishell: %s: is a directory\n", line->command[0]);
		exit (126);
	}
	else // 디렉토리가 아니라면 line->command[0] 는 일반파일, 명령어, 파일없음 셋 중 하나.
	{
		if (line->command[0][ft_strlen(line->command[0]) - 1] == '/') // 맨 끝 '/' 유무 확인
		{
			ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
			line->command[0][ft_strlen(line->command[0]) - 1] = '\0'; // '/' 을 '\0' 로 대체해서 파일 검사 시 '/' 제거 효과.
			if (!access(line->command[0], F_OK)) // 파일 유무 확인
			{
				printf("minishell: %s: Not a directory\n", line->command[0]);
				exit (126);
			}
			else // 파일이 없다면
			{
				printf("minishell: %s: No such file or directory\n", line->command[0]);
				exit (127);
			}
		}
		else // 맨 끝에 '/' 없다면
		{

			if ((!access(current_path, F_OK) || !access(line->command[0], F_OK)) && !ft_strchr(line->command[0], '/')) // current_path, line->command 둘 중 하나라도 파일이 있다면
			{
				execve(current_path, line->command, env); // current_path 로 파일 실행.
				// current_path 실패
				ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
				if (!access(current_path, F_OK) && access(line->command[0], X_OK) == -1) // current_path 에 파일이 있다면
				{
					printf("minishell: %s: Permission denied\n", line->command[0]);
					exit (126);
				}
				else // current_path 에 파일이 없다면
				{
					printf("minishell: %s: command not found\n", line->command[0]);
					exit (127);
				}
			}
			else // 둘 다 파일이 없거나 '/' 문자가 있다면
			{
				ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
				if (ft_strchr(current_path, '/') || ft_strchr(line->command[0], '/'))
				{
					if (S_ISREG(filestat.st_mode))
					{
						if (access(line->command[0], X_OK) == -1)
							printf("minishell: %s: Permission denied\n", line->command[0]);
						else if (access(line->command[0], X_OK) == 0)
							execve(line->command[0], line->command, env);
						exit (126);
					}
					printf("minishell: %s: No such file or directory\n", line->command[0]);
					exit (127);
				}
				else
				{
					printf("minishell: %s: command not found\n", line->command[0]);
					exit (127);
				}
			}
		}
	}
	ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
	printf("minishell: %s: No such file or directory\n", line->command[0]);
	exit (127);
}