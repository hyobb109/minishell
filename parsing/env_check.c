/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/26 22:35:14 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_envkey(const char *s1, const char *s2)
{
	size_t	i;
	char	*str1;
	char	*str2;

	str1 = (char *)s1;
	str2 = (char *)s2;
	i = 0;
	while (str1[i] != ENVIRON || str2[i])
	{
		if (str1[i] != str2[i])
			return (0);
		i++;
	}
	return (1);
}

void	env_trans(char *str, int i, t_edeque *envp, int flag)
{
	t_env	*tmp;

	tmp = envp->head;
	while (tmp)
	{
		if (is_envkey(str + i, tmp->key))
		{
			if (flag == FREE)
			{
				free(str);
				str = ft_strdup(tmp->val);
			}
			else
			{
				ft_memcpy(str, tmp->val, ft_strlen(tmp->val));
			}
			return ;
		}
		tmp = tmp->next;
	}
}

void	search_env(char **cmd, t_edeque *envp)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] == ENVIRON)
			{
				env_trans(cmd[i], j + 1, envp, FREE);
			}
			j++;
		}
		i++;
	}
}

// char	*search_env(char *str, t_edeque *envp)
// {
// 	int	i;
// 	int	quote;
	
// 	i = 0;
// 	quote = 0;
// 	while (is_blank(str[i]))
// 		i++;
// 	while (str[i])
// 	{
		
		
// 	}
// }