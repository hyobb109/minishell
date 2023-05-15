/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_parents.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:33:30 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/15 14:14:27 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parents_process(t_deque *cmd_deque)
{
	t_token	*current_token;
	int		(*fd)[2];
	int		count;

	current_token = cmd_deque->head;
	count = cmd_deque->cnt - 1;
	fd = NULL;
	if (find_here_doc(cmd_deque))
		return ;
	while (current_token != NULL)
	{
		check_file(current_token);
		current_token = current_token->next;
	}
	create_pipe(&fd, cmd_deque);
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

void	only_builtins(t_deque *cmd_deque, int (*fd)[2])
{
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (manage_file(cmd_deque->head) > 0)
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
		cur_token->pid = ft_fork();
		if (!cur_token->pid)
		{
			ft_signal_child();
			child_process(cur_token, count, total, fd);
		}
		++count;
		cur_token = cur_token->next;
	}
	signal(SIGINT, signal_handler_parent);
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
			if (is_heredoc(cur_file))
			{
				unlink(cur_file->filename);
			}
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
}

char	*check_env_var(char *line, t_edeque *envp)
{
	t_vars	v;
	char	buf[ARG_MAX];

	init_vars(&v, buf, 0);
	while (line[v.i])
	{
		if (line[v.i] == '$')
		{
			v.i += env_trans(&line[v.i + 1], envp, &buf[v.len], v);
			v.len = ft_strlen(buf);
		}
		else
		{
			buf[v.len++] = line[v.i];
		}
		v.i++;
	}
	return (ft_strdup(buf));
}
