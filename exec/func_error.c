/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:16:57 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/03 22:28:18 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	ft_fork()
{
	pid_t	res;

	res = fork();
	if (res == -1)
		ft_error();
	return (res);
}

int	ft_dup(int fd)
{
	int	res;

	res = dup(fd);
	if (res == -1)
		ft_error();
	return (res);
}

int	ft_dup2(int old_fd, int new_fd)
{
	int	res;

	res = dup2(old_fd, new_fd);
	if (res == -1)
		ft_error();
	return (res);
}

int	ft_close(int fd)
{
	int	res;

	res = close(fd);
	if (res == -1)
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