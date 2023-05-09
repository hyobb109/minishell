/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_pipe_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:40:43 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/09 19:29:43 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_pipe(int (**fd)[2], t_deque *cmd_deque)
{
	int	idx;

	if (cmd_deque->cnt == 1)
		return ;
	(*fd) = malloc(sizeof(int *) * (cmd_deque->cnt - 1));
	if (!(*fd))
		ft_error();
	idx = 0;
	while (idx < cmd_deque->cnt - 1)
	{
		pipe((*fd)[idx]);
		++idx;
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
