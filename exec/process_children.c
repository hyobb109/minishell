/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/27 21:35:12 by hyunwoju         ###   ########.fr       */
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
	(void) line;
	//TODO - builtin 상의
	//int debugging = exec_builtins(line);
	//printf("debugging : %d\n", debugging);
	//TODO - builtin 상의/
	manage_pipe(count, total, fd);
	manage_file(line);
	//execute_line(line, count, total, fd);
}

void	manage_file(t_token *line)
{
	t_fdata	*cur_file;
	int		infile_fd;
	int		outfile_fd;

	cur_file = line->files;
	while (cur_file != NULL)
	{
		if (cur_file->type == INFILE)
		{
			open_infile(cur_file->filename, &infile_fd);
		}
		else if (cur_file->type == OUTFILE)
		{
			open_outfile(cur_file->filename, &outfile_fd);
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

void	open_outfile(char *filename, int *outfile_fd)
{
	*outfile_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (*outfile_fd == -1)
	{
		printf("%s: %s\n", filename, strerror(errno));
		exit (1);
	}
}

//void	check_error_infile(char *filename, int *infile)
//{
//	if (!access(filename, R_OK))
//	{
//		if (*infile)
//			close(*infile);
//		*infile = open(filename, O_RDONLY);
//		return ;
//	}
//	if (!access(filename, F_OK))
//	{
//		printf("%s: Permission denied\n", filename);
//		exit (1);
//	}
//	printf("%s: No such file or directory\n", filename);
//	exit (1);
//}

//void	check_error_outfile(char *filename, int *outfile)
//{
//	if (*outfile)
//		close(*outfile);
//	*outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
//	if (*outfile == -1)
//	{
//		printf("%s: Permission denied\n", filename);
//		exit (1);
//	}
//}

//void	execute_line(t_token *line, int count, int total, int (*fd)[2])
//{

//}

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