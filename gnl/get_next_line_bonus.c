/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 20:13:01 by yunjcho           #+#    #+#             */
/*   Updated: 2022/10/26 21:26:56 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_info	*head;
	t_info			*node;
	char			*result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	node = ft_get_node(&head, fd);
	ft_init(node, &result);
	while (1)
	{
		if (node->index >= node->rbyte)
		{
			result = join(result, node);
			node->rbyte = read(fd, node->buffer, BUFFER_SIZE);
			if (node->rbyte <= 0)
				return (ft_exception(node, result, &head));
			node->index = 0;
		}
		node->len++;
		if (node->buffer[node->index++] == '\n')
		{
			result = join(result, node);
			return (result);
		}
	}
}

void	ft_init(t_info *node, char **result)
{
	*result = malloc(1);
	**result = 0;
	node->total = 0;
}

t_info	*ft_get_node(t_info **head, int fd)
{
	t_info	*lst;

	lst = *head;
	while (lst)
	{
		if (lst->fd == fd)
			return (lst);
		lst = lst->next;
	}
	return (ft_lstadd_back(head, fd));
}

t_info	*ft_lstadd_back(t_info **head, int fd)
{
	t_info	*new_node;
	t_info	*lst;

	new_node = malloc(sizeof(t_info));
	if (new_node)
	{
		new_node->fd = fd;
		new_node->index = 0;
		new_node->rbyte = 0;
		new_node->len = 0;
		new_node->total = 0;
		new_node->next = 0;
	}
	lst = *head;
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
		lst->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}
