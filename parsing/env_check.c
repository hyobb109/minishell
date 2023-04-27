/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/27 22:40:12 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_envkey(const char *s1, const char *s2, size_t *key_len)
{
	size_t	i;
	char	*str1;
	char	*str2;

	str1 = (char *)s1;
	str2 = (char *)s2;
	i = 0;
	while ((str1[i] != ENVIRON && !is_blank(str1[i]) && str1[i] != '\'' && str1[i]) || str2[i])
	{
		if (str1[i] != str2[i])
		{
			while (str1[i] != ENVIRON && !is_blank(str1[i]) && str1[i] != '\'' && str1[i])
			{
				i++;
				// printf("str1: %s str1[%zu]: %c\n", str1, i, str1[i]);
				*key_len = i;
			}
			return (0);
		}
		i++;
	}
	*key_len = i;
	return (1);
}
// $USer 
// $USER Hyobicho
int	env_trans(char *str, int i, t_edeque *envp, char *buf)
{
	t_env	*tmp;
	size_t	key_len;

	key_len = 0;
	tmp = envp->head;
	while (tmp)
	{
		if (is_envkey(str + i, tmp->key, &key_len))
		{
			ft_memcpy(buf,tmp->val,ft_strlen(tmp->val));
			return (key_len);
		}
		tmp = tmp->next;
	}
	// printf("key not found.. key_len: %zu\n", key_len);
	return (key_len);
}

void	search_env(char **cmds, t_edeque *envp)
{
	int		i;
	int		j;
	int		k;
	char	buf[ARG_MAX];

	i = 0;
	while (cmds[i])
	{
		j = 0;
		k = 0;
		if (ft_strchr(cmds[i], ENVIRON))
		{
			ft_memset(buf, 0, ARG_MAX);
			while (cmds[i][j])
			{
				// $ 만나면 치환
				if (cmds[i][j] == ENVIRON)
				{
					// 환경변수 키 글자수 만큼 인덱스 늘려줌
					j += env_trans(cmds[i], j + 1, envp, &buf[k]);
					// printf("cmds[%d][%d]: %c, buf: %s\n", i, j, cmds[i][j], buf);
					k = ft_strlen(buf);
				}
				// 따옴표 안 글자들 버퍼에 복사
				else
				{
					buf[k++] = cmds[i][j];
				}
				j++;
			}
			free(cmds[i]);
			cmds[i] = ft_strdup(buf);
		}
		i++;
	}
}
