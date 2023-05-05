/* ************************************************************************** */
/*                                                                             */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/01 22:27:32 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// 덱의 token 들은 각각의 자식프로세스가 실행할 라인
// 먼저 덱을 순회
// < 다음에 오는 infile 들의 존재여부 확인
// 만약 infile 이 없다면, 해당위치 뒤의 > outfile 은 open 하지 않고 error flag on, 에러 처리 할 파일명 저장
// 만약 infile 들이 전부 존재한다면, > 바로 전 infile and 가장 끝의 infile 만 open 하고 fd 저장
// 위에서 문제가 없었을 경우 > 다음에 오는 outfile 일단 전부 open
// 가장 끝의 outfile and < 바로 전 outfile 제외 close 하고 이 둘의 fd 저장
// << 히어독이 여러 개 들어오면 갯수만큼 임시파일 열어서 입력받지만 실행할 때는 마지막 히어독만 적용시킨다.
// 명령어 error 여도 outfile 은 open 한다.
// outfile permission error 이면 바로 브레이크
// "< a wc -l > b < c > d"  -> wc 명령어가 c 를 입력으로 받는다.
// "< a > b < c > d" 에서 b가 permission error 라면 d 는 open 되지 않는다.
// 에러처리 순서 -> 파일 먼저 그 다음 명령어

void	child_process(t_token *line, int count, int total, int (*fd)[2])
{
	char **env;

	env = NULL;
	manage_file(line);
	manage_pipe(count, total, fd);
	manage_io(line, count, total, fd);
	if (line->func == BUILTIN)
	{
		exec_builtins(line); //TODO - builtins return(-1); 처리
		exit(0);
	}
	else
	{
		env = make_envstrs(line);
		if (line->command != NULL)
			execute_line(line, env);
	}
	exit (0);
}

char	**make_envstrs(t_token *token)
{
	int		idx;
	char	**strs;
	t_env	*tmp;

	strs = malloc(sizeof(char *) * (token->envp->cnt + 1));
	if (!strs)
		exit(1);
	idx = 0;
	tmp = token->envp->head;
	while (tmp)
	{
		strs[idx] = ft_strjoin_three(tmp->key, "=", tmp->val);
		tmp = tmp->next;
		idx++;
	}
	strs[idx] = 0;
	return (strs);
}

void	execute_line(t_token *line, char **env)
{
	char	*path_env;
	char	**path;
	char	*current_path;
	char	*part_path;
	int		i;
	//struct stat filestat;
	path_env = ft_getenv(line->envp, "PATH"); // value
	if (!path_env)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
		printf("minishell: %s: No such file or directory\n", line->command[0]);
		exit (127);
	}
	printf("******line->command[0]******: %s\n", line->command[0]);
	path = ft_split(path_env, ':');
	//int result = ft_strchr_idx(line->command[0], '.');
	//stat(line->command[0], &filestat);
    //if(result >= 0 && S_ISDIR(filestat.st_mode))
	//{
	//	ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
	//	printf("minishell: %s: is a directory\n", line->command[0]);
	//	exit(126);
    //}
	//else if (result >= 0 && S_ISREG(filestat.st_mode))
	//{
	//	ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
	//	printf("minishell: %s: Permission denied\n", line->command[0]);
	//	exit(126);
	//}
		//printf("st_mode : %d\n", filestat.st_mode);
		//if (result >= 0)
		//{
		//	if (chdir(line->command[0]) == -1)
		//	{
		//		ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
		//		printf("minishell: %s: %s\n", line->command[0], strerror(errno));
		//		exit(1);
		//	}
		//}
		//ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
		//printf("minishell: %s: No such file or directory\n", line->command[0]);
		//exit(126);
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
	printf("******current_path******: %s\n", current_path);
	execve(line->command[0], line->command, env);
    //stat(current_path, &filestat);
    //if(S_ISDIR(filestat.st_mode))
	//{
	//	ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
	//	printf("minishell: %s: is a directory\n", line->command[0]);
	//	exit(126);
    //}
	if (access(current_path, X_OK) && access(current_path, F_OK) == 0)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
		printf("minishell: %s: Permission denied\n", line->command[0]);
		exit(126);
	}
	execve(current_path, line->command, env);
	ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
	printf("minishell: %s: command not found\n", line->command[0]);
	exit (127);
}

void	manage_io(t_token *line, int count, int total, int (*fd)[2])
{
	if (line->infile_fd)
	{
		ft_dup2(line->infile_fd, STDIN_FILENO, line->func);
		ft_close(line->infile_fd, line->func);
	}
	else if (count != 0)
	{
		ft_dup2(fd[count - 1][0], STDIN_FILENO, line->func);
		ft_close(fd[count - 1][0], line->func);
	}
	if (line->outfile_fd)
	{
		ft_dup2(line->outfile_fd, STDOUT_FILENO, line->func);
		ft_close(line->outfile_fd, line->func);
	}
	else if (count != total - 1)
	{
		ft_dup2(fd[count][1], STDOUT_FILENO, line->func);
		ft_close(fd[count][1], line->func);
	}
}

void	manage_file(t_token *line)
{
	t_fdata	*cur_file;
	int		infile_fd;
	int		outfile_fd;
	int		append_flag;

	cur_file = line->files;
	infile_fd = 0;
	outfile_fd = 0;
	while (cur_file != NULL)
	{
		append_flag = 0;
		if (cur_file->type == INFILE || cur_file->type == LIMITER || cur_file->type == Q_LIMITER)
		{
			open_infile(cur_file->filename, &infile_fd, line->func);
		}
		else if (cur_file->type == OUTFILE || cur_file->type == APPEND)
		{
			if (cur_file->type == APPEND)
				append_flag = 1;
			open_outfile(cur_file->filename, &outfile_fd, append_flag, line->func);
		}
		cur_file = cur_file->next;
	}
	line->infile_fd = infile_fd;
	line->outfile_fd = outfile_fd;
}

int	open_infile(char *filename, int *infile_fd, int func)
{
	*infile_fd = open(filename, O_RDONLY);
	if (ft_strchr(filename, BLANK))
	{
		printf("%s: %s\n", filename, "ambiguous redirect");
		exit (1);
	}
	if (*infile_fd == -1)
	{
		printf("%s: %s\n", filename, strerror(errno));
		if (func != BUILTIN)
			exit (1);
		else
			return (-1);
	}
	return (0);
}

int	open_outfile(char *filename, int *outfile_fd, int append_flag, int func)
{
	if (append_flag)
		*outfile_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		*outfile_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (ft_strchr(filename, BLANK))
	{
		printf("%s: %s\n", filename, "ambiguous redirect");
		exit (1);
	}
	if (*outfile_fd == -1)
	{
		printf("%s: %s\n", filename, strerror(errno));
		if (func != BUILTIN)
			exit (1);
		else
			return (-1);
	}
	return (0);
}

void	manage_pipe(int count, int total, int (*fd)[2])
{
	int	idx;

	idx = 0;
	while (idx < total - 1)
	{
		if (!(count == idx) && !(count == idx + 1))
		{
			ft_close(fd[idx][0], GENERAL);
			ft_close(fd[idx][1], GENERAL);
		}
		else
		{
			if (count == 0)
				ft_close(fd[0][0], GENERAL);
			else if (count == total - 1)
				ft_close(fd[count - 1][1], GENERAL);
			else if (count == idx + 1)
				ft_close(fd[idx][1], GENERAL);
			else if (count == idx)
				ft_close(fd[idx][0], GENERAL);
		}
		++idx;
	}
}
