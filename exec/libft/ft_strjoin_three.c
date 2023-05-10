/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 16:35:21 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/09 19:34:12 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	int		idx1;
	int		idx2;
	int		str_len;
	char	*result;

	idx1 = 0;
	idx2 = 0;
	if (!s1 || !s2 || !s3)
		return (0);
	str_len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	result = malloc(sizeof(char) * (str_len + 1));
	if (!result)
		ft_error();
	while (s1[idx1])
		result[idx2++] = s1[idx1++];
	idx1 = 0;
	while (s2[idx1])
		result[idx2++] = s2[idx1++];
	idx1 = 0;
	while (s3[idx1])
		result[idx2++] = s3[idx1++];
	result[idx2] = '\0';
	return (result);
}
