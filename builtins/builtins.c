/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:53:24 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 21:09:41 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_builtins(t_token *token)
{
	int		result;
	char	*tmp;

	result = 0;
	g_exit_status = 0;
	tmp = ft_strlowcase(ft_strdup(token->command[0]));
	if (!ft_strcmp(tmp, "echo"))
		result = exec_echo(token);
	else if (!ft_strcmp(tmp, "pwd"))
		result = exec_pwd();
	else if (!ft_strcmp(tmp, "env"))
		result = exec_env(token);
	else if (!ft_strcmp(token->command[0], "cd"))
		result = exec_cd(token);
	else if (!ft_strcmp(token->command[0], "export"))
		result = exec_export(token);
	else if (!ft_strcmp(token->command[0], "unset"))
		result = exec_unset(token);
	else if (!ft_strcmp(token->command[0], "exit"))
	{
		exec_exit(token);
		result = 1;
	}
	free(tmp);
	return (result);
}
