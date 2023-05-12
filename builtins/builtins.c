/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:53:24 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/12 17:13:32 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_builtins(t_token *token)
{
	char	*tmp;

	if (ft_strcmp(token->command[0], "exit"))
		g_exit_status = 0;
	tmp = ft_strlowcase(ft_strdup(token->command[0]));
	if (!ft_strcmp(tmp, "echo"))
		exec_echo(token);
	else if (!ft_strcmp(tmp, "pwd"))
		exec_pwd();
	else if (!ft_strcmp(tmp, "env"))
		exec_env(token);
	else if (!ft_strcmp(token->command[0], "cd"))
		exec_cd(token);
	else if (!ft_strcmp(token->command[0], "export"))
		exec_export(token);
	else if (!ft_strcmp(token->command[0], "unset"))
		exec_unset(token);
	else if (!ft_strcmp(token->command[0], "exit"))
		exec_exit(token);
	free(tmp);
}
