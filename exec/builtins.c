/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/21 22:54:30 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(void)
{
	char	cwd_name[PATH_MAX];

	getcwd(cwd_name, sizeof(cwd_name));
	ft_putendl_fd(cwd_name, 1);
	return (1);
}

int	exec_exit(t_token *token)
{
	//TODO - 프로그램 종료시 반환값 전달 가능한지 확인 후 리턴값 없애기
	//TODO - 프로그램 종료 실패 시 예외처리 추가?
	ft_putstr_fd("exit\n", 1);
	return (!kill(token->pid, SIGKILL));
}

char	*join_all(char **strs, int idx)
{
	char	*result;

	result = ft_strdup("\0");
	while (strs[idx])
	{
		result = ft_strjoin(result, strs[idx]);
		if (strs[idx + 1])
			result = ft_strjoin(result, " ");
		idx++;
	}

	if (result[0] == '\0')
	{
		free(result);
		return (0);
	}
	return (result);
}

// echo 명령어에서 출력을 시작할 행의 인덱스를 반환
int	check_option(char **arguments)
{
	int	idx;

	idx = 0;
	if (!arguments[1])
		return (1);
	if (!strncmp(arguments[1], "-n", 2))
	{
		idx = 2;
		while (arguments[1][idx])
		{
			if (arguments[1][idx] != 'n')
				return (1);
			idx++;
		}
	}
	else
		return (1);
	return (2);
}

void	print_args(char **arguments, int target_idx)
{
	int	i;

	i = 0;
	char *tmp = join_all(arguments, target_idx);
	printf("tmp: %s\n", tmp);
	if (target_idx == 1)
		ft_putendl_fd(tmp, 1);
	else
		ft_putstr_fd(tmp, 1);
	free(tmp);
	// while(arguments[target_idx])
	// {
	// 	ft_putstr_fd(arguments[target_idx], 1);
	// 	target_idx++;
	// 	if (arguments[target_idx] != 0)
	// 		ft_putstr_fd(" ", 1);
	// }
}

int	exec_echo(t_token *token)
{
	int		target_idx;

	// token->command[1] = "abc ";
	// token->command[2] = "def";
	// if (token->command[1])
		// arguments = parse_args(token->command);

	//TODO - Debuggin 추후 삭제
	// int i = 0;
	// while (arguments[i])
	// {
	// 	printf("arguments[%d] %s\n", i, arguments[i]);
	// 	i++;
	// }

	target_idx = check_option(token->command);
	printf("idx: %d\n", target_idx);
	print_args(token->command, target_idx);
	// if (target_idx == )
	// {
	// 	// printf("yes\n");
	// 	print_args(arguments, target_idx); //-n 옵션이 유효한 경우 (-n은 출력 안함)
	// }
	// else
	// {
	// 	// printf("no\n");
	// 	ft_putendl_fd(echo->args, 1); //-n 옵션이 무효한 경우 (-n 출력)
	// }
	return (1);
}

int	exec_builtins(t_token *token)
{
	if (!strcmp(ft_strlowcase(token->command[0]), "echo"))
		return(exec_echo(token));
	else if (!strcmp(ft_strlowcase(token->command[0]), "pwd"))
		return (exec_pwd());
	else if (!strcmp(ft_strlowcase(token->command[0]), "env"))
		return (1);
	else if (!strcmp(token->command[0], "cd"))
		return (1);
	else if (!strcmp(token->command[0], "export"))
		return (1);
	else if (!strcmp(token->command[0], "unset"))
		return (1);
	else if (!strcmp(token->command[0], "exit"))
		return (exec_exit(token));
	return (0);
}
