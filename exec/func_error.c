/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:16:57 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/12 17:17:40 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	ft_fork(void)
{
	pid_t	res;

	res = fork();
	if (res == -1)
		ft_error();
	return (res);
}

int	ft_dup2(int old_fd, int new_fd, int func)
{
	int	res;

	res = dup2(old_fd, new_fd);
	if (res == -1 && func != P_BUILTIN)
		ft_error();
	return (res);
}

int	ft_close(int fd, int func)
{
	int	res;

	res = close(fd);
	if (res == -1 && func != P_BUILTIN)
		ft_error();
	return (res);
}

int	ft_pipe(int fd[2])
{
	int	res;

	res = pipe(fd);
	if (res == -1)
		ft_error();
	return (res);
}

void	print_error(t_token *line, char *err_msg, int exit_code)
{
	ft_dup2(STDERR_FILENO, STDOUT_FILENO, line->func);
	printf("minishell: %s: %s\n", line->command[0], err_msg);
	if (line->func != P_BUILTIN)
		exit(exit_code);
	else
		return ;
}
