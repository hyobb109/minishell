/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/16 21:34:50 by hyobicho         ###   ########.fr       */
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

void	parse_args(char *str)
{
	char	quote;
	int		i;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote && str[i] == quote)
		{
			quote = 0;
			str[i] = -1;
		}
		else if (quote == 0 && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
			str[i] = -1;
		}
		i++;
	}
}

int	exec_echo(t_token *echo)
{
	// char	result[1000];
	// args parsing
	// TODO : echo->args $? => 마지막 작업 종료상태 출력
	// parse_args(echo->args, result);
	if (echo->args)
		parse_args(echo->args);
	// printf("args: %s\n", echo->args);
	if (echo->option)
		ft_putstr_fd(echo->args, 1);
	else
		ft_putendl_fd(echo->args, 1);
	return (1);
}

int	exec_builtins(t_token *token)
{
	if (!strcmp(ft_strlowcase(token->command), "echo"))
		return(exec_echo(token));
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
