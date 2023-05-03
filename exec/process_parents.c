/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_parents.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:33:30 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/03 15:32:52 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	only_builtins(t_deque *cmd_deque,  int (*fd)[2])
{
	int	stdin_fd;
	int	stdout_fd;
	int	result;

	result = 0;
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	result = exec_builtins(cmd_deque->head);
	if (result != -1)
	{
		manage_file(cmd_deque->head);
		manage_io(cmd_deque->head, 0, 1, fd);
		if (cmd_deque->head->infile_fd)
		{
			dup2(stdin_fd, STDIN_FILENO);
			close(stdin_fd);
		}
		if (cmd_deque->head->outfile_fd)
		{
			dup2(stdout_fd, STDOUT_FILENO);
			close(stdout_fd);
		}
	}
}

void	parents_process(t_deque *cmd_deque)
{
	t_token	*current_token;
	int		(*fd)[2];
	int		count;

	current_token = cmd_deque->head;
	count = cmd_deque->cnt - 1;
	find_here_doc(cmd_deque);
	while (current_token != NULL)
	{
		check_file(current_token);
		current_token = current_token->next;
	}
	fd = create_pipe(cmd_deque);
	if (cmd_deque->cnt == 1 && cmd_deque->head->func == BUILTIN)
	{
		only_builtins(cmd_deque, fd); //TODO - builtins return(-1); 처리
		return ;
	}
	create_child(cmd_deque, fd);
	close_pipe(fd, count);
	free(fd);//pipe(fds) free()
	wait_child(count, cmd_deque);
	unlink_here_doc(cmd_deque);
}

void	unlink_here_doc(t_deque *cmd_deque)
{
	t_token	*cur_token;
	t_fdata	*cur_file;

	cur_token = cmd_deque->head;
	while (cur_token != NULL)
	{
		cur_file = cur_token->files;
		while (cur_file != NULL)
		{
			if (cur_file->type == DELIMITER || cur_file->type == Q_DELIMITER)
			{
				unlink(cur_file->filename);
			}
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
}

void	find_here_doc(t_deque *cmd_deque)
{
	t_token	*cur_token;
	t_fdata *cur_file;
	int		count;

	cur_token = cmd_deque->head;
	count = 0;
	while (cur_token != NULL)
	{
		cur_file = cur_token->files;
		while (cur_file != NULL)
		{
			if (cur_file->type == DELIMITER || cur_file->type == Q_DELIMITER)
			{
				open_here_doc(cur_file, count);
				++count;
			}
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
}

void	open_here_doc(t_fdata *cur_file, int count)
{
	char	here_doc_fd;
	char	*count_to_char;
	char	*here_doc_name;
	char	*line;
	
	count_to_char = ft_itoa(count);
	here_doc_name = ft_strjoin("/tmp/here_doc_temp", count_to_char);
	free(count_to_char);
	here_doc_fd = open(here_doc_name, O_RDWR | O_CREAT, 0777);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!ft_strncmp(line, cur_file->filename, ft_strlen(cur_file->filename)))
		{
			if (*(line + ft_strlen(cur_file->filename)) == '\n')
			{
				free(line);
				break ;
			}
		}
		ft_putstr_fd(line, here_doc_fd);
		free(line);
	}
	ft_memset(cur_file->filename, 0, sizeof(char) * strlen(cur_file->filename));
	ft_memcpy(cur_file->filename, here_doc_name, ft_strlen(here_doc_name));
	free(here_doc_name);
	close(here_doc_fd);
}

void	check_file(t_token *line)
{
	t_fdata	*current_point;

	current_point = line->files;
	while (current_point != NULL)
	{
		if (current_point->type == INFILE)
		{
			if (check_infile(current_point->filename))
				break ;
		}
		else if (current_point->type == OUTFILE)
		{
			if (check_outfile(current_point->filename))
				break ;
		}
		current_point = current_point->next;
	}
}

int	check_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (TRUE);
	close(fd);
	return (FALSE);
}

int	check_outfile(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		return (TRUE);
	close(fd);
	return (FALSE);
}

void	wait_child(int count, t_deque *cmd_deque)
{
	int		idx;
	int 	status;
	pid_t	pid;

	idx = 0;
	while (idx < count + 1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			exit(EXIT_FAILURE);
		find_child(cmd_deque, status, pid);
		++idx;
	}
}

void	find_child(t_deque *cmd_deque, int status, pid_t pid)
{
	t_token *cur_point;

	cur_point = cmd_deque->head;
	while (cur_point != NULL)
	{
		if (cur_point->pid == pid)
		{
			cur_point->status = status;
			printf("exit status = %d\n", WEXITSTATUS(status));
		}
		cur_point = cur_point->next;
	}
}

void	close_pipe(int (*fd)[2], int count)
{
	int	idx;

	idx = 0;
	while (idx < count)
	{
		close(fd[idx][0]);
		close(fd[idx][1]);
		++idx;
	}
}

void	create_child(t_deque *cmd_deque, int (*fd)[2])
{
	t_token	*cur_token;
	int		count;
	int		total;

	count = 0;
	total = cmd_deque->cnt;
	cur_token = cmd_deque->head;
	while (count < total)
	{
		cur_token->pid = fork();
		if (cur_token->pid == -1)
			exit (1);
		else if (!cur_token->pid)
			child_process(cur_token, count, total, fd);
		++count;
		cur_token = cur_token->next;
	}
}

int (*create_pipe(t_deque *cmd_deque))[2]
{
	int (*fd)[2];
	int	idx;

	if (cmd_deque->cnt == 1)
		return (NULL);
	fd = malloc(sizeof(int *) * (cmd_deque->cnt - 1));
	if (!fd)
		ft_error();
	idx = 0;
	while (idx < cmd_deque->cnt - 1)
	{
		pipe(fd[idx]);
		++idx;
	}
	return (fd);
}
