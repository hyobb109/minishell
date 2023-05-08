/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:42:23 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 23:05:29 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_envlist(t_token *token)
{
	char	*str;
	t_env	*tmp;

	str = NULL;
	tmp = token->envp->head;
	while (tmp)
	{
		if (!tmp->val)
		{
			tmp = tmp->next;
			continue ;
		}
		str = ft_strjoin_three(tmp->key, "=", tmp->val);
		printf("%s\n", str);
		free(str);
		tmp = tmp->next;
	}
}

int	exec_env(t_token *token)
{
	print_envlist(token);
	return (1);
}
