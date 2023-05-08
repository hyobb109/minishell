/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:37:29 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 18:37:57 by hyunwoju         ###   ########.fr       */
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
		exit(0);
	}
	else
	{
		env = make_envstrs(line);
		if (line->command != NULL)
			execute_line(line, env);
	}
	exit (0);
}

char	**make_envstrs(t_token *token)
{
	int		idx;
	char	**strs;
	t_env	*tmp;

	strs = malloc(sizeof(char *) * (token->envp->cnt + 1));
	if (!strs)
		exit(1);
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
