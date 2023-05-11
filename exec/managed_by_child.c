/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   managed_by_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:46:11 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/11 13:52:03 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	manage_file(t_token *line)
{
	t_fdata	*cur_file;
	int		infile_fd;
	int		outfile_fd;

	cur_file = line->files;
	infile_fd = 0;
	outfile_fd = 0;
	while (cur_file != NULL)
	{
		if (cur_file->type == INFILE || is_heredoc(cur_file))
		{
			if (open_infile(cur_file->filename, &infile_fd, line->func) < 0)
				return (-1);
		}
		else if (cur_file->type == OUTFILE || cur_file->type == APPEND)
		{
			if (open_outfile(cur_file, &outfile_fd, line->func) < 0)
				return (-1);
		}
		cur_file = cur_file->next;
	}
	line->infile_fd = infile_fd;
	line->outfile_fd = outfile_fd;
	return (1);
}

int	open_infile(char *filename, int *infile_fd, int func)
{
	*infile_fd = open(filename, O_RDONLY);
	if (*infile_fd == -1)
	{
		printf("minishell: %s: %s\n", filename, strerror(errno));
		if (func != P_BUILTIN)
			exit (1);
		else
			return (-1);
	}
	return (1);
}

int	open_outfile(t_fdata *cur_file, int *outfile_fd, int func)
{
	int	apd_flag;

	apd_flag = 0;
	if (cur_file->type == APPEND)
		apd_flag = 1;
	if (apd_flag)
		*outfile_fd = open(cur_file->filename, \
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*outfile_fd = open(cur_file->filename, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*outfile_fd == -1)
	{
		printf("minishell: %s: %s\n", cur_file->filename, strerror(errno));
		if (func != P_BUILTIN)
			exit (1);
		else
			return (-1);
	}
	return (1);
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
