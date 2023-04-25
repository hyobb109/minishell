/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/25 19:43:43 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// buf에 환경변수 치환해서 복사하고 str index는 치환한 부분만큼 증가 && 복사한 길이 리턴
int	env_trans(char *str, int *idx, char *buf, t_edeque *envp)
{
	int		len;
	int		n;
	t_env	*tmp;

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
	n = len;
	tmp = envp->head;
	while (n)
	{
		while (tmp)
		{
			if (!ft_strncmp(str, tmp->key, n))
			{
				ft_memcpy(buf, tmp->val, ft_strlen(tmp->val) + 1);
				*idx += n;
				// printf("buf : %s\n", buf);
				return (ft_strlen(buf));
			}
			tmp = tmp->next;
		}
		n--;
	}
	 // 환경변수 목록에 없으면 버퍼 복사 안하고 idx만 증가시킴
	*idx += len;
	return (0);
}
