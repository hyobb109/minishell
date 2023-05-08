/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_pipe_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:40:43 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/08 17:41:03 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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