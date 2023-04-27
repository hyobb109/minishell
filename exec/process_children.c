/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/27 14:11:04 by hyunwoju         ###   ########.fr       */
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

void	child_process(t_token *line, int count, int total, int (*fd)[2])
{
	//TODO - builtin 상의
	int debugging = exec_builtins(line);
	printf("debugging : %d\n", debugging);
	//TODO - builtin 상의/
	manage_pipe(count, total, fd);
	check_file(line);
}

void	check_file(t_token *line)
{
	t_fdata	*current_point;
	int		cur_infile;
	int		cur_outfile;
	int		result;

	current_point = line->files;
	cur_infile = 0;
	cur_outfile = 0;
	while (current_point != NULL)
	{
		if (ft_strcmp(current_point->type, "infile"))
		{
			if (check_error_infile(current_point->filename, &cur_infile))
				break ;
		}
		else if (ft_strcmp(current_point->type, "outfile"))
		{
			if (check_error_outfile(current_point->filename, &cur_outfile))
				break ;
		}
		current_point = current_point->next;
	}
	line->infile_fd = cur_infile;
	line->outfile_fd = cur_outfile;
}

int	check_error_infile(char *filename, int *infile)
{
	if (!access(filename, R_OK))
	{
		*infile = open(filename, O_RDONLY);
		return (FALSE);
	}
	if (!access(filename, F_OK))
	{
		printf("%s: Permission denied\n", filename);
		return (TRUE);
	}
	printf("%s: No such file or directory\n", filename);
}

int	check_error_outfile(char *filename, int *outfile)
{
	
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