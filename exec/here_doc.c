/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:26:40 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/09 20:32:15 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_here_doc(t_deque *cmd_deque)
{
	t_token	*cur_token;
	t_fdata	*cur_file;
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

void	exec_here_doc(t_token *cur_token, t_fdata *cur_file, \
						char *here_doc_name)
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
		if (!ft_strncmp(line, cur_file->filename, \
			ft_strlen(cur_file->filename)))
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

void	here_doc_write(t_token *cur_token, t_fdata *cur_file, \
						char *line, int here_doc_fd)
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
