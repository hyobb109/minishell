/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/26 16:15:45 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// buf에 환경변수 치환해서 복사하고 str index는 치환한 부분만큼 증가 && 복사한 길이 리턴
int	env_trans(char *str, int *idx, char *buf, t_edeque *envp)
{
	int		i;
	int		n;
	int		quote;
	t_env	*tmp;

	i = 0;
	quote = 0;
	// while (str[i])
	// {
	// 	if (!quote && is_blank(str[i]))
	// 		break;
	// 	if (!quote && str[i] == '$' || str[i] == '<' || str[i] == '>')
	// 		break;
	// 	if (!quote && str[i] == '\'' || str[i] == '\"')
	// 		quote = str[i];
	// 	i++;
	// }
	while (str[i] && !(is_blank(str[i]) || str[i] == '$' || str[i] == '<' || str[i] == '>' || str[i] == '\'' || str[i] == '\"'))
		i++;
	// printf("===========\n");
	printf("***i: %d, str: %s ***buf: %p\n", i, str+i, buf);
	if (i == 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (0);
		buf[0] ='$';
		return (1);
	}
	n = i;
	tmp = envp->head;
	while (n)
	{
		while (tmp)
		{
			if (!ft_strcmp(str, tmp->key))
			{
				ft_memcpy(buf, tmp->val, ft_strlen(tmp->val) + 1);
				*idx += n;
				printf("*str: %s, n: %d, buf : %s\n", str, n, buf);
				return (ft_strlen(buf));
			}
			tmp = tmp->next;
		}
		n--;
	}
	 // 환경변수 목록에 없으면 버퍼 복사 안하고 idx만 증가시킴
	*idx += i;
	return (0);
}
