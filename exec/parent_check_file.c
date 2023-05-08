/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_check_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:28:41 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/08 17:29:10 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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