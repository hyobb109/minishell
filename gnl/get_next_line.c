/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 20:13:01 by yunjcho           #+#    #+#             */
/*   Updated: 2022/10/26 16:57:28 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_info	node = {"\0", 0, 0, 0, 0};
	char			*result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	ft_init(&node, &result);
	while (1)
	{
		if (node.index >= node.rbyte)
		{
			result = join(result, &node);
			node.rbyte = read(fd, node.buffer, BUFFER_SIZE);
			if (node.rbyte <= 0)
				return (ft_exception(&node, result));
			node.index = 0;
		}
		node.len++;
		if (node.buffer[node.index++] == '\n')
		{
			result = join(result, &node);
			return (result);
		}
	}
}
