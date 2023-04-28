/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/28 19:52:29 by hyunwoju         ###   ########.fr       */
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
	//TODO - builtin 상의
	//int debugging = exec_builtins(line);
	//printf("debugging : %d\n", debugging);
	//TODO - builtin 상의/
	manage_pipe(count, total, fd);
	manage_file(line);
	manage_io(line, count, total, fd);
	env = make_envlist(line);
	if (line->command[0][0] != '\0')
		execute_line(line, env);
}

char	**make_envlist(t_token *token)
{
	int		idx;
	char	**str;
	t_env	*tmp;

	idx = 0;
	str = (char **)malloc(sizeof(char *) + (token->envp->cnt + 1));
	tmp = token->envp->head;
	while (tmp)
	{
		if (!tmp->val)
			str[idx] = ft_strdup(tmp->key); 
		else
			str[idx] = ft_strjoin_three(tmp->key, "=", tmp->val);
		tmp = tmp->next;
		idx++;
	}
	str[idx] = 0;
	return (str);
}

void	execute_line(t_token *line, char **env)
{
	char	*path_env;
	char	**path;
	char	*current_path;
	char	*part_path;
	int		i;
	path_env = ft_getenv(line->envp, "PATH"); // value
	path = ft_split(path_env, ':');
	execve(line->command[0], line->command, env);
	i = 0;
	while (path[i] != 0)
	{
		part_path = ft_strjoin(path[i], "/");
		current_path = ft_strjoin(part_path, line->command[0]);
		if (!access(current_path, F_OK))
			break ;
		free(current_path);
		++i;
	}
	if (execve(current_path, line->command, env) == - 1)
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("%s: command not found\n", line->command[0]);
	}
	exit (1);
}

void	manage_io(t_token *line, int count, int total, int (*fd)[2])
{
	if (line->infile_fd)
	{
		dup2(line->infile_fd, STDIN_FILENO);
		close(line->infile_fd);
	}
	else
	{
		if (count != 0)
		{
			dup2(fd[count - 1][0], STDIN_FILENO);
			close(fd[count - 1][0]);
		}
	}
	if (line->outfile_fd)
	{
		dup2(line->outfile_fd, STDOUT_FILENO);
		close(line->outfile_fd);
	}
	else
	{
		if (count != total - 1)
		{
			dup2(fd[count][1], STDOUT_FILENO);
			close(fd[count][1]);
		}
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
		if (cur_file->type == INFILE)
		{
			open_infile(cur_file->filename, &infile_fd);
		}
		else if (cur_file->type == OUTFILE || cur_file->type == APPEND)
		{
			if (cur_file->type == APPEND)
				append_flag = 1;
			open_outfile(cur_file->filename, &outfile_fd, append_flag);
		}
		cur_file = cur_file->next;
	}
	line->infile_fd = infile_fd;
	line->outfile_fd = outfile_fd;
}

void	open_infile(char *filename, int *infile_fd)
{
	*infile_fd = open(filename, O_RDONLY);
	if (*infile_fd == -1)
	{
		printf("%s: %s\n", filename, strerror(errno));
		exit (1);
	}
}

void	open_outfile(char *filename, int *outfile_fd, int append_flag)
{
	if (append_flag)
		*outfile_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		*outfile_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (*outfile_fd == -1)
	{
		printf("%s: %s\n", filename, strerror(errno));
		exit (1);
	}
}

void	manage_pipe(int count, int total, int (*fd)[2])
{
	int	idx;

	idx = 0;
	while (idx < total - 1)
	{
		if (!(count == idx) && !(count == idx + 1))
		{
			close(fd[idx][0]);
			close(fd[idx][1]);
		}
		else
		{
			if (count == 0)
				close(fd[0][0]);
			else if (count == total - 1)
				close(fd[count - 1][1]);
			else
			{
				if (count == idx + 1)
					close(fd[idx][1]);
				else if (count == idx)
					close(fd[idx][0]);
			}
		}
		++idx;
	}
}