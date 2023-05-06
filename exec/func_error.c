/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:16:57 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/06 14:51:21 by yunjcho          ###   ########.fr       */
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