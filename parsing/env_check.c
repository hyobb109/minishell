/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/24 05:49:14 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// buf에 환경변수 치환해서 복사하고 str index는 치환한 부분만큼 증가 && 복사한 길이 리턴
int	env_trans(char *str, int *idx, char *buf, char **env)
{
	int		len;
	int		i;
	char **tmp;
	int		n;

	len = 0;
	while (str[len] && !(is_blank(str[len]) || str[len] == '$' || str[len] == '<' || str[len] == '>' || str[len] == '\'' || str[len] == '\"'))
		len++;
	// printf("===========\n");
	// printf("***len: %d, ***buf: %p\n", len, buf);
	if (!len)
	{
		if (str[len] == '\'' || str[len] == '\"')
			return (0);
		buf[0] ='$';
		return (1);
	}
	i = 0;
	n = len;
	while (n)
	{
		while (env[i])
		{
			tmp = ft_split(env[i], "=");
			// printf("key: %s, val: %s\n", tmp[0], tmp[1]);
			if (!ft_strncmp(str, tmp[0], n))
			{
				ft_memcpy(buf, tmp[1], ft_strlen(tmp[1]) + 1);
				free_strs(tmp);
				*idx += n;
				// printf("buf : %s\n", buf);
				return (ft_strlen(buf));
			}
			free_strs(tmp);
			i++;
		}
		n--;
	}
	 // 환경변수 목록에 없으면 버퍼 복사 안하고 idx만 증가시킴
	*idx += len;
	return (0);
}
