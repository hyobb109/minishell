/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/16 20:05:34 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	exec_pwd(void)
{
	// 디렉토리 최대 길이(나중에 제대로 확인)
	char	cwd_name[256];
	// printf("token->command: %s\n", token->command);
	getcwd(cwd_name, sizeof(cwd_name));
	ft_putendl_fd(cwd_name, 1);
	return (1);
}

int	exec_builtins(t_token *token)
{
	if (!strcmp(ft_strlowcase(token->command), "echo"))
		return (1);
	else if (!strcmp(ft_strlowcase(token->command), "pwd"))
		return (exec_pwd());
	else if (!strcmp(ft_strlowcase(token->command), "env"))
		return (1);
	else if (!strcmp(token->command, "cd"))
		return (1);
	else if (!strcmp(token->command, "export"))
		return (1);
	else if (!strcmp(token->command, "unset"))
		return (1);
	else if (!strcmp(token->command, "exit"))
		return (1);
	return (0);
}
