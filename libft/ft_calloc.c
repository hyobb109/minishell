/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:21:35 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 19:35:42 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t				total;
	unsigned char		*res;

	total = count * size;
	if (count != 0 && total / count != size)
		return (0);
	res = (unsigned char *)malloc(total);
	if (!res)
		ft_error();
	return (ft_memset(res, 0, total));
}
