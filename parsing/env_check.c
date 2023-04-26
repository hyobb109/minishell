/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/26 18:34:46 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_trans(char **cmd, t_edeque *envp)
{
	int		i;
	t_env	*tmp;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == ENVIRON)
		{
			tmp = envp->head;
			while (tmp)
			{
				if (!ft_strcmp(&cmd[i][1], tmp->key))
				{
					free(cmd[i]);
					cmd[i] = ft_strdup(tmp->val);
					return ;
				}
				tmp = tmp->next;
			}
		}
		i++;
	}
}
