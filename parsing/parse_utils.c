/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:49:00 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 15:54:14 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_environ(char quote, char c)
{
	if (c == '$' && quote == CLOSED)
		return (TRUE);
	if (c == '$' && quote == '\"')
		return (TRUE);
	return (FALSE);
}

void	init_vars(t_vars *v, char *buf, int file)
{
	if (file)
		ft_memset(buf, 0, PATH_MAX);
	else
		ft_memset(buf, 0, ARG_MAX);
	v->flag = file;
	v->i = 0;
	v->len = 0;
	v->quote = CLOSED;
}

void	free_files(t_fdata **lst)
{
	t_fdata	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
}

void	append_file(t_fdata **head, t_fdata *new)
{
	t_fdata	*last;

	if (!(*head))
	{
		*head = new;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new;
}
