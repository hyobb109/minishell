/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/12 17:01:39 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_token *line, int count, int total, int (*fd)[2])
{
	char	**env;

	env = NULL;
	manage_file(line);
	manage_pipe(count, total, fd);
	manage_io(line, count, total, fd);
	if (line->func == BUILTIN)
	{
		exec_builtins(line);
		exit(line->status);
	}
	else
	{
		env = make_envstrs(line);
		if (line->command && line->command[0])
			execute_line(line, env);
	}
	exit (line->status);
}

char	**make_envstrs(t_token *token)
{
	int		idx;
	char	**strs;
	t_env	*tmp;

	strs = malloc(sizeof(char *) * (token->envp->cnt + 1));
	if (!strs)
		ft_error();
	idx = 0;
	tmp = token->envp->head;
	while (tmp)
	{
		strs[idx] = ft_strjoin_three(tmp->key, "=", tmp->val);
		tmp = tmp->next;
		idx++;
	}
	strs[idx] = 0;
	return (strs);
}
