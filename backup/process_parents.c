/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_parents.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:33:30 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 17:23:29 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	only_builtins(t_deque *cmd_deque,  int (*fd)[2])
{
	int	stdin_fd;
	int	stdout_fd;

	(void)fd;
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (manage_file(cmd_deque->head))
	{
		manage_io(cmd_deque->head, 0, 1, fd);
		exec_builtins(cmd_deque->head);
	}
	if (cmd_deque->head->infile_fd)
	{
		ft_dup2(stdin_fd, STDIN_FILENO, cmd_deque->head->func);
		ft_close(stdin_fd, cmd_deque->head->func);
	}
	if (cmd_deque->head->outfile_fd)
	{
		ft_dup2(stdout_fd, STDOUT_FILENO, cmd_deque->head->func);
		ft_close(stdout_fd, cmd_deque->head->func);
	}	
}

void	parents_process(t_deque *cmd_deque)
{
	t_token	*current_token;
	int		(*fd)[2];
	int		count;

	current_token = cmd_deque->head;
	count = cmd_deque->cnt - 1;
	if (find_here_doc(cmd_deque))
		return ;
	while (current_token != NULL)
	{
		check_file(current_token);
		current_token = current_token->next;
	}
	fd = create_pipe(cmd_deque);
	if (cmd_deque->cnt == 1 && cmd_deque->head->func == BUILTIN)
	{
		cmd_deque->head->func = P_BUILTIN;
		only_builtins(cmd_deque, fd);
		return ;
	}
	create_child(cmd_deque, fd);
	close_pipe(fd, count);
	free(fd);
	wait_child(count, cmd_deque);
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
			if (cur_file->type == LIMITER || cur_file->type == Q_LIMITER)
			{
				unlink(cur_file->filename);
			}
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
}

int	find_here_doc(t_deque *cmd_deque)
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
			if (cur_file->type == LIMITER || cur_file->type == Q_LIMITER)
			{
				if (open_here_doc(cur_token, cur_file, count))
					return (1);
				++count;
			}
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
	return (0);
}

int	open_here_doc(t_token *cur_token, t_fdata *cur_file, int count)
{
	char	*here_doc_name;
	pid_t	pid;
	
	here_doc_name = here_doc_make_name(count);
	pid = ft_fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		exec_here_doc(cur_token, cur_file, here_doc_name);
	}
	signal(SIGINT, signal_handler_heredoc);
	waitpid(-1, &g_exit_status, 0);
	ft_signal_set();
	if (WIFSIGNALED(g_exit_status))
		return (1);
	ft_memset(cur_file->filename, 0, sizeof(char) * strlen(cur_file->filename));
	ft_memcpy(cur_file->filename, here_doc_name, ft_strlen(here_doc_name));
	free(here_doc_name);
	return (0);
}

char	*here_doc_make_name(int count)
{
	char	*count_to_char;
	char	*here_doc_name;
	
	while (1)
	{
		count_to_char = ft_itoa(count);
		here_doc_name = ft_strjoin("/tmp/here_doc_temp", count_to_char);
		free(count_to_char);
		if (access(here_doc_name, F_OK))
			return (here_doc_name);
		free(here_doc_name);
		++count;
	}
}

void	exec_here_doc(t_token *cur_token, t_fdata *cur_file, char *here_doc_name)
{
	int		here_doc_fd;
	char	*line;

	here_doc_fd = open(here_doc_name, O_RDWR | O_CREAT, 0777);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
		{
			exit(0);
		}
		if (!ft_strncmp(line, cur_file->filename, ft_strlen(cur_file->filename)))
		{
			if (*(line + ft_strlen(cur_file->filename)) == '\n')
			{
				free(line);
				break ;
			}
		}
		here_doc_write(cur_token, cur_file, line, here_doc_fd);
	}
	ft_close(here_doc_fd, cur_token->func);
	exit (0);
}

void	here_doc_write(t_token *cur_token, t_fdata *cur_file, char *line, int here_doc_fd)
{
	char	*tmp;
	
	if (cur_file->type == LIMITER)
		tmp = check_env_var(line, cur_token->envp);
	else
		tmp = line;
	ft_putstr_fd(tmp, here_doc_fd);
	free(line);
	if (cur_file->type == LIMITER)
		free(tmp);
}

char	*check_env_var(char *line, t_edeque *envp)
{
	int		idx;
	char	buf[ARG_MAX];
	int		len;
	
	ft_memset(buf, 0, ARG_MAX);
	idx = 0;
	len = 0;
	while (line[idx])
	{
		if (line[idx] == '$')
		{
			idx += env_trans(&line[idx + 1], envp, &buf[len], 1);
			len = ft_strlen(buf);
		}
		else
		{
			buf[len++] = line[idx];
		}
		idx++;
	}
	return (ft_strdup(buf));
}

void	check_file(t_token *line)
{
	t_fdata	*current_point;

	current_point = line->files;
	while (current_point != NULL)
	{
		if (current_point->type == INFILE)
		{
			if (check_infile(current_point->filename, line))
				break ;
		}
		else if (current_point->type == OUTFILE)
		{
			if (check_outfile(current_point->filename, line))
				break ;
		}
		current_point = current_point->next;
	}
}

int	check_infile(char *filename, t_token *line)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (TRUE);
	ft_close(fd, line->func);
	return (FALSE);
}

int	check_outfile(char *filename, t_token *line)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		return (TRUE);
	ft_close(fd, line->func);
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
	g_exit_status = cmd_deque->tail->status;
}

void	find_child(t_deque *cmd_deque, int status, pid_t pid)
{
	t_token *cur_point;

	cur_point = cmd_deque->head;
	while (cur_point != NULL)
	{
		if (cur_point->pid == pid)
		{
			if (WIFSIGNALED(status))
				cur_point->status = WTERMSIG(status);
			else
				cur_point->status = status;
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
		ft_close(fd[idx][0], GENERAL);
		ft_close(fd[idx][1], GENERAL);
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
	signal(SIGINT, signal_handler);
	while (count < total)
	{
		cur_token->pid = ft_fork();
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