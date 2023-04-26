/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_deque.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 14:45:34 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 14:46:44 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	make_envlst(t_edeque *envp, char **env)
{
	int		i;
	t_env	*env_node;
	char	**tmp;

	i = 0;
	init_edeque(envp);
	while (env[i])
	{
		env_node = malloc(sizeof(t_env));
		if (!env_node)
			ft_error();
		tmp = ft_split(env[i], "=");
		env_node->key = ft_strdup(tmp[0]);
		env_node->val = ft_strdup(tmp[1]);
		free_strs(tmp);
		append_back_env(envp, env_node);
		i++;
	}
	// print_edeque(envp);
}
