/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/25 20:23:00 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_token *line, int count, int total, int (*fd)[2])
{
	(void) line;
	manage_pipe(count, total, fd);
}

void	manage_pipe(int count, int total, int (*fd)[2])
{
	int	idx;

	idx = 0;
	while (idx < total - 1)
	{
		if (!(count == idx) && !(count == idx + 1))
		{
			close(fd[idx][0]);
			close(fd[idx][1]);
		}
		else
		{
			if (count == 0)
				close(fd[0][0]);
			else if (count == total - 1)
				close(fd[count - 1][1]);
			else
			{
				if (count == idx + 1)
					close(fd[idx][1]);
				else if (count == idx)
					close(fd[idx][0]);
			}
		}
		++idx;
	}
}
