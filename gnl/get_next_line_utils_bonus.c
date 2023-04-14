/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 19:02:09 by yunjcho           #+#    #+#             */
/*   Updated: 2022/10/26 18:37:48 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	ft_memcpy(char *dst, char *src, size_t len)
{
	size_t	i;

	if (!dst || !src)
		return ;
	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

char	*join(char *old, t_info *node)
{
	char	*new;

	new = malloc(node->total + node->len + 1);
	if (new)
	{
		if (node->total)
			ft_memcpy(new, old, node->total);
		ft_memcpy(new + node->total,
			&node->buffer[node->index - node->len], node->len);
		node->total += node->len;
		node->len = 0;
		new[node->total] = '\0';
		free(old);
	}
	return (new);
}

char	*ft_exception(t_info *node, char *result, t_info **head)
{
	char	*ret;

	ret = 0;
	if (node->rbyte == 0 && node->total)
		ret = result;
	else
		free(result);
	ft_remove_node(head, node->fd);
	return (ret);
}

void	ft_remove_node(t_info **head, int fd)
{
	t_info	*lst;
	t_info	*tmp;

	lst = *head;
	while (lst->next)
	{
		if (lst->next->fd == fd)
		{
			tmp = lst->next;
			lst->next = lst->next->next;
			free(tmp);
		}
		else
			lst = lst->next;
	}
	lst = *head;
	if (lst->fd == fd)
	{
		*head = lst->next;
		free(lst);
	}
}
